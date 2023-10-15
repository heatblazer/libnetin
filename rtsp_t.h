#ifndef RTSP_T_H
#define RTSP_T_H
#include "defs.h"
#include "types.h"
#include "utils.h"
#include "tjson.hpp"
#include <pcap/pcap.h>
namespace rtsp
{
    struct RtspRFC : public IParseable<Result_t, RtspRFC>
    {
    public:
        RtspRFC() = delete;

        RtspRFC(const IParseable::type& res);

        RtspRFC& operator()(const IParseable::type&);

        RtspRFC& operator()();

        inline Result_t::TypeRFC type() const { return Result_t::TypeRFC::RTSP;}

    };

} //ns rtsp
#endif // RTSP_T_H
