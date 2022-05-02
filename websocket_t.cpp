#include "websocket_t.h"
#include "mqtt_t.h"
#include "utils.h"
#include <cstring>
namespace websocket {

//mqtt can travel over websocket, so passing a mqtt here will also be needed

WebSocketRFC::~WebSocketRFC() noexcept {}

WebSocketRFC::WebSocketRFC(const IParseable::type &res)
    : IParseable<Result_t,WebSocketRFC>{res} , m_maskKey{0}
{}


WebSocketRFC &WebSocketRFC::operator()(const IParseable::type &res MAYBEUNUSED)
{
    struct EthL4 eth = utils::GetEthL4(res.data);
    MAYBEUNUSED  size_t offset = 0;
    MAYBEUNUSED  size_t total = res.out.len;
    MAYBEUNUSED unsigned int payload_len = res.out.caplen - eth.payload_len;
    switch (eth.type) {
    case EthL4::TCP: {
        // peel off
        offset = eth.options_len + (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(tcphdr));
        const char *pdata = (const char*)res.data+offset;
        if (pdata && (unsigned char)pdata[0] == 0x82) {
            unsigned int weblen = (unsigned char)(pdata[1] & 0x7F);
            auto mqtt = mqtt::MqttRFC{res};
            mqtt.WebSocketOffset = (payload_len - weblen);
            jsonb.add(tjson::JsonField{"protocol", "WebSocket"});
            if (mqtt.WebSocketOffset > 2) {
                mqtt.WebSocketMask.value = utils::tobin<unsigned int>(pdata+2);
                jsonb.add(tjson::JsonField{"mask", mqtt.WebSocketMask.value});
                Valid = true;
            }
            mqtt();
        }
        break;
    }
    case EthL4::UDP: //Websocket can't go over udp - fall trough default
    case EthL4::UNKNOWN:
    default:
        break;
    }
    return *this;
}


WebSocketRFC& WebSocketRFC ::operator()()
{
    return this->operator()(this->value);
}



} //ns websocket
