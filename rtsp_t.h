#ifndef RTSP_T_H
#define RTSP_T_H
#include "types.h"
#include <pcap/pcap.h>

namespace rtsp
{
    struct RtspRFC : public IParseable<Result_t, RtspRFC>
    {
    public:

        enum eRtspCommands
        {
            OPTIONS,
            DESCRIBE,
            SETUP,
            PLAY,
            TEARDOWN,
            PAUSE,
            SET_PARAMETER,
            GET_PARAMETER,
            UNKNOWN,
            SIZE = UNKNOWN // convinience

        };

        RtspRFC() = delete;

        RtspRFC(const IParseable::type& res);

        RtspRFC& operator()(const IParseable::type&);

        RtspRFC& operator()();

        inline Result_t::TypeRFC type() const { return Result_t::TypeRFC::RTSP;}
    private:

        eRtspCommands get_command(const char* pdata, std::string_view& out);
    private:
        std::vector<std::string> m_fields;
        uint8_t m_Payload[1500]; //mtu


    };

} //ns rtsp
#endif // RTSP_T_H
