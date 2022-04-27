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
    size_t offset MAYBEUNUSED = 0;
    size_t total = res.out.len;
    unsigned char data[14] = {0};
    switch (eth.type) {
    case EthL4::UDP: {
        offset = (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(udphdr));
        memcpy(data, res.data+offset,total-offset);
        std::cout << "UDP MQTT\r\n";
        break;
    }
    case EthL4::TCP: {
        offset = (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(tcphdr));
        memcpy(data, res.data+offset,total-offset);
        std::cout << "TCP MQTT\r\n";
        break;
    }
    case EthL4::UNKNOWN: //fall trough
    default:
        break;
    }
    return *this;
}



} //mqtt
