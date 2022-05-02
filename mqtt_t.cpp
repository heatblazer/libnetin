#include "mqtt_t.h"
#include "utils.h"
#include "Pcap.h"
#include <cstring>

#ifndef __unix__
    #include <malloc.h>
#else
    #include <alloca.h>
#endif

namespace mqtt {

MqttRFC::MqttRFC(const IParseable::type &res)
    :IParseable<Result_t,MqttRFC>{res},
    WebSocketOffset{0},
    WebSocketMask{0}
{
    memset(&m_header, 0, sizeof(m_header));
}

MqttRFC::~MqttRFC() noexcept
{
    if (m_header.cilentIDLen ) {
        //free(m_header.payload);
        //m_header.payload = nullptr;
    }
}

MqttRFC &MqttRFC::operator()()
{
    return this->operator()(this->value);
}

MqttRFC &MqttRFC ::operator()(const IParseable::type &res)
{
    struct EthL4 eth = utils::GetEthL4(res.data);
    MAYBEUNUSED  size_t offset = 0;
    MAYBEUNUSED  size_t total = res.out.len;
    MAYBEUNUSED unsigned char data[14]  = {0};
    MAYBEUNUSED unsigned int payload_len = res.out.caplen - eth.payload_len;

    switch (eth.type) {
    case EthL4::UDP: {
        offset = (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(udphdr));
        break;
    }      
    case EthL4::TCP: {
        offset = WebSocketOffset + eth.options_len + (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(tcphdr));

        const char *pdata = (const char*)res.data+offset;
        if (WebSocketMask.value) {
            char* masked = (char*)res.data+offset;
            char* begin = masked;
            WebSocketMask.value = SWAP4(WebSocketMask.value);
            for(unsigned int i = 0; i < payload_len; i++) {
                unsigned char mid = WebSocketMask.data[i % 4];
                *(masked++) ^= mid;
            }
            pdata = begin;
        }
        //always add connections
        jsonb.add(tjson::JsonField{"srcip", eth.sourceIP});
        jsonb.add(tjson::JsonField{"dstip", eth.destIP});

        m_header.header = (pdata == nullptr) ? 0x00 : pdata[0];
        if (m_header.header == 0x10 && payload_len > 16) {
            m_header.nameLen = ((pdata[2] << 8) | pdata[3]);
            if (m_header.nameLen > 10)
                return *this;
            ::memcpy(m_header.protocolName, pdata + 4, m_header.nameLen);
            m_header.protocolVersion = (unsigned char) *(pdata+4+m_header.nameLen);
            m_header.uConnectFlags.value = *(pdata+4+m_header.nameLen+1);
            m_header.cilentIDLen = ((pdata[14] << 8) | pdata[15]);
            jsonb.add(tjson::JsonField{"protocol", "MQTT"});
            jsonb.add(tjson::JsonField{"name", m_header.protocolName});
            jsonb.add(tjson::JsonField{"version", m_header.protocolVersion});
            if (m_header.cilentIDLen) {
                memcpy(m_header.payload, pdata+16, m_header.cilentIDLen);
                jsonb.add(tjson::JsonField{"payload", m_header.payload});
            }
            //it came from websocket...
            if (WebSocketMask.value && WebSocketOffset) {
                libnetin::Pcap::getSerializer().Add(jsonb);
            }
            Valid = true;
        }
        break;
    }
    case EthL4::UNKNOWN: //fall trough
    default:
        break;
    }
    return *this;
}



} //mqtt
