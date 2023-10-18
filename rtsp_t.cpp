#include "rtsp_t.h"
#include "utils.h"
#include <cstring>

namespace rtsp
{

RtspRFC::RtspRFC(const IParseable::type& res)
:  IParseable<Result_t,RtspRFC>{res}
{

}

RtspRFC &RtspRFC::operator()(const IParseable::type& res)
{
    PRINTDBG("rtsp_t.cpp");

    struct EthL4 eth = utils::GetEthL4(res.data);
    MAYBEUNUSED  size_t offset = 0;
    MAYBEUNUSED  size_t total = res.out.len;
    switch (eth.type) {
        case EthL4::TCP: {
            offset = eth.options_len + (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(tcphdr));
            const char *pdata = (const char*)res.data+offset;
            if (std::strstr(pdata, "RTSP") || std::strstr(pdata, "rtsp")) {
                std::string_view cmd;
                MAYBEUNUSED RtspRFC::eRtspCommands r = get_command(pdata, cmd);
                jsonb.add(tjson::JsonField{"protocol", "RTSP"});
                jsonb.add(tjson::JsonField{"command", cmd.data()}); //tjson does not support toString for string_veiw so use a cdata
                Valid = true;
            }
        }
        FALLTROUGH;
        case EthL4::UDP: //RTSP can go over UDP - TODO:// handle this
        case EthL4::UNKNOWN:
        default:
            break;
    }

    return *this;
}

RtspRFC::eRtspCommands RtspRFC::get_command(const char *pdata, std::string_view& out)
{
    RtspRFC::eRtspCommands result;
    if (std::strstr(pdata, "OPTIONS")) {
        result = RtspRFC::OPTIONS;
        out = "OPTIONS";
    } else if (std::strstr(pdata, "SETUP")) {
        result = RtspRFC::SETUP;
        out = "SETUP";
    } else if (std::strstr(pdata, "PLAY")) {
        result = RtspRFC::PLAY;
        out = "PLAY";
    } else if (std::strstr(pdata, "TEARDOWN")) {
        result = RtspRFC::TEARDOWN;
        out = "TEARDOWN";
    } else if (std::strstr(pdata, "DESCRIBE")) {
        result = RtspRFC::DESCRIBE;
        out = "DESCRIBE";
    } else {
        result = RtspRFC::UNKNOWN;
        out = "UNKNOWN";
    }
    return result;
}

RtspRFC &RtspRFC::operator()()
{
    return this->operator()(this->value);
}

}
