#ifndef MQTT_T_H
#define MQTT_T_H
#include "defs.h"
#include "types.h"
#include "tjson.hpp"

namespace mqtt {

struct MqttRFC: public IParseable<Result_t, MqttRFC>
{

    struct {
        unsigned char header;
        unsigned short msgLen;
        unsigned short nameLen;
        unsigned char* protocolName;
        unsigned char protocolVersion;
        union {
            struct {
                uint8_t Username : 1;
                uint8_t Password : 1;
                uint8_t WillRetain : 1;
                uint8_t QoSLevel : 1;
                uint8_t WillFlag : 1;
                uint8_t CleanSession : 1;
                uint8_t Reserved : 1;
            } s_field;
            uint8_t value;
        } uConnectFlags;
        unsigned short keepAlive;
        unsigned short cilentIDLen;
        unsigned char* payload;
    } m_header;

public:
    MqttRFC() = delete;

    explicit MqttRFC(const IParseable::type& res);

    MqttRFC &operator()(const IParseable::type &res);

    MqttRFC& operator()();

    Result_t::TypeRFC type() const { return Result_t::TypeRFC::MQTT;}


};

} //mqtt
#endif // MQTT_T_H
