#include "mqtt_t.h"
#include "utils.h"
#include <cstring>

#ifndef __unix__
    #include <malloc.h>
#else
    #include <alloca.h>
#endif

//json usage: jsonb.add(tjson::JsonField{"protocol", "TURN"});

namespace mqtt {

MqttRFC::MqttRFC(const IParseable::type &res)
        :IParseable<Result_t,MqttRFC>{res}
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
    switch (eth.type) {
    case EthL4::UDP: {
        offset = (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(udphdr));
        break;
    }
    case EthL4::TCP: {
        offset = eth.options_len + (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(tcphdr));
        const char *pdata = (const char*)res.data+offset;
        m_header.header = pdata == nullptr ? 0x00 : pdata[0];
        if (m_header.header == 0x10 ) {
            m_header.nameLen = ((pdata[2] << 8) | pdata[3]);
            ::strncpy(m_header.protocolName, pdata + 4, m_header.nameLen);
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
