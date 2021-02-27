#ifndef RTP_T_H
#define RTP_T_H
#include "defs.h"
#include "Pcap.h"
#include <pcap/pcap.h>

namespace rtp {

    struct rtp_t
    {
        unsigned char meta[4];
        unsigned int timestamp;
        unsigned int SSRC;
        unsigned int CSRC;
    };

    class RtpRFC
    {
    public:
        RtpRFC();

        bool valid() const;

        RtpRFC& operator()(const Pcap::Result_t&res);

    private:
        template<typename T>
        void parse(T data);
    };

} // rtp


#endif // RTP_T_H
