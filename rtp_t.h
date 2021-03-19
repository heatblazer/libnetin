#ifndef RTP_T_H
#define RTP_T_H
#include "defs.h"
#include "types.h"
#include "tjson.hpp"
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

    struct RtpRFC : public IParseable<Result_t>
    {
    public:
        RtpRFC();

        RtpRFC(const IParseable::type& res);

        RtpRFC& operator()(const IParseable::type &res);

        RtpRFC& operator()();

        Result_t::TypeRFC type() const { return Result_t::TypeRFC::RTP;}


    private:

        template<typename T>
        void parse(T data);

        bool m_valid;

        struct {
            unsigned int cc;
            unsigned int v;
            unsigned int p;
            unsigned int x;
            unsigned int m;
            unsigned int pt;
            unsigned int timestamp;
            unsigned int ssrc;
            unsigned int csrc;
        } m_fields;
    };

} // rtp


#endif // RTP_T_H
