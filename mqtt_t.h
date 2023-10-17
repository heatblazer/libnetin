#ifndef MQTT_T_H
#define MQTT_T_H
#include "defs.h"
#include "types.h"

namespace mqtt {

struct MqttRFC: public IParseable<Result_t, MqttRFC>
{

    struct {
        unsigned char header;
        unsigned short msgLen;
        unsigned short nameLen;
        char protocolName[16];
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
            } PACKED s_field ;
            uint8_t value;
        } uConnectFlags;
        unsigned short keepAlive;
        unsigned short cilentIDLen;
        char payload[1024];
    } PACKED m_header ;

public:
    MqttRFC() = delete;

    ~MqttRFC() noexcept ;

    explicit MqttRFC(const IParseable::type& res);

    MqttRFC &operator()(const IParseable::type &res);

    MqttRFC& operator()();

    inline Result_t::TypeRFC type() const { return Result_t::TypeRFC::MQTT;}

    unsigned int WebSocketOffset;

    union {
        unsigned int value;
        unsigned char data[sizeof(unsigned int)];
    } WebSocketMask;
};

} //mqtt
#endif // MQTT_T_H
