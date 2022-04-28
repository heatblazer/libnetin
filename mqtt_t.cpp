#include "mqtt_t.h"
#include "utils.h"
#include <cstring>

//json usage: jsonb.add(tjson::JsonField{"protocol", "TURN"});

namespace mqtt {

MqttRFC::MqttRFC(const IParseable::type &res)
        :IParseable<Result_t,MqttRFC>{res}
{
    memset(&m_header, 0, sizeof(m_header));
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
        break;
    }
    case EthL4::UNKNOWN: //fall trough
    default:
        break;
    }
    return *this;
}



} //mqtt
