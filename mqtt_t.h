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
        unsigned short remainingLen;
        unsigned short protocolNameLen;
        unsigned char nameVer[5];
        unsigned char connectFlags;
        unsigned short keepAlive;
        unsigned short length;
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
