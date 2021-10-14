#include "rtcp_t.h"
#include <string.h> //memset

namespace rtcp {

    RtcpRFC::RtcpRFC(const IParseable::type &res)
        : IParseable<Result_t,RtcpRFC>{res}
    {
        memset(&m_fields, 0, sizeof(m_fields));
    }

    RtcpRFC &RtcpRFC::operator()(const IParseable::type &res)
    {
        return *this;
    }

    RtcpRFC &RtcpRFC::operator()()
    {
        return this->operator()(this->value);
    }



}
