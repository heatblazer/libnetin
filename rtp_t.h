#ifndef RTP_T_H
#define RTP_T_H
#include "defs.h"
#include "parse.h"
#include "Pcap.h"
#include <pcap/pcap.h>

namespace rtp {

    /*
    The RTP header has the following format:

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |V=2|P|X|  CC   |M|     PT      |       sequence number         |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           timestamp                           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           synchronization source (SSRC) identifier            |
    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    |            contributing source (CSRC) identifiers             |
    |                             ....                              |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
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
        struct {
            int cc;
            int v;
            int p;
            int x;
            int m;
            int pt;
            int timestamp;
            int ssrc;
            int csrc;
        } m_fields;

    };

} // rtp


#endif // RTP_T_H
