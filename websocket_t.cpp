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


WebSocketRFC &WebSocketRFC::operator()(const IParseable::type &res)
{

    PRINTDBG("websocket_t.cpp");

    struct EthL4 eth = utils::GetEthL4(res.data);
    MAYBEUNUSED  size_t offset = 0;
    MAYBEUNUSED  size_t total = res.out.len;
    switch (eth.type) {
    case EthL4::TCP: {
        // peel off
        offset = eth.options_len + (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(tcphdr));
        const char *pdata = (const char*)res.data+offset;
        if (pdata && (unsigned char)pdata[0] == WebSocketRFC::StartHeader)
        {
            unsigned int weblen = (unsigned char)(pdata[1] & WebSocketRFC::PayloadLenMask);
            auto mqtt = mqtt::MqttRFC{res};
            mqtt.WebSocketOffset = weblen; 
            jsonb.add(tjson::JsonField{"protocol", "WebSocket"});
            if (mqtt.WebSocketOffset > WebSocketRFC::MaskOffset) {
                mqtt.WebSocketMask.value = utils::tobin<unsigned int>(pdata+WebSocketRFC::MaskOffset);
                jsonb.add(tjson::JsonField{"mask", mqtt.WebSocketMask.value});
            }
            if (mqtt().Valid) {
                //in case we have mqtt serialized - add it here
                if (mqtt.WebSocketMask.value && mqtt.WebSocketOffset) {
                    jsonb.add(tjson::JsonField{"WebSocket->MQTT", true});
                    for (const auto &json : mqtt.jsonb.fields) {
                        jsonb.add(json);
                    }
                }
            }
            Valid = true;
        }
        break;
    }
    FALLTROUGH;
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
