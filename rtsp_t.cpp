#include "rtsp_t.h"

namespace rtsp
{

RtspRFC::RtspRFC(const IParseable::type& res)
:  IParseable<Result_t,RtspRFC>{res}
{

}

RtspRFC &RtspRFC::operator()(const IParseable::type& res)
{
    struct EthL4 eth = utils::GetEthL4(res.data);
    MAYBEUNUSED  size_t offset = 0;
    MAYBEUNUSED  size_t total = res.out.len;
    switch (eth.type) {
        case EthL4::TCP: {
            offset = eth.options_len + (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(tcphdr));
            const char *pdata = (const char*)res.data+offset;
            //handle json data here TODO
        }
        case EthL4::UDP:
        case EthL4::UNKNOWN:
        default:
            break;
    }

    return *this;
}

RtspRFC &RtspRFC::operator()()
{
    return this->operator()(this->value);
}

}
