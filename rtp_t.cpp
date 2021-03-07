#include "rtp_t.h"

#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>

#include <iostream>

namespace rtp {

    RtpRFC::RtpRFC() : m_valid{false}
    {

    }

    RtpRFC::RtpRFC(const IParseable::type &res) : IParseable<Result_t>{res},
        m_valid{false}
    {

    }

    bool RtpRFC::valid() const
    {
        return m_valid;
    }

    RtpRFC& RtpRFC::operator()()
    {
        return this->operator()(this->value);
    }


    RtpRFC &RtpRFC::operator()(const IParseable::type &res)
    {
        const struct ether_header* ethernetHeader;
        const struct ip* ipHeader;
        const struct udphdr* udpHeader;
        char sourceIP[INET_ADDRSTRLEN];
        char destIP[INET_ADDRSTRLEN];
        if (!res.data)
            return *this;
        ethernetHeader = (struct ether_header*)res.data;
        if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {
            ipHeader = (struct ip*)(res.data + sizeof(struct ether_header));
            inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIP, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(ipHeader->ip_dst), destIP, INET_ADDRSTRLEN);
            if (ipHeader->ip_p == IPPROTO_UDP) {
                udpHeader = (struct udphdr*)(res.data + sizeof(struct ether_header) + sizeof(struct ip));
                int offset = sizeof(struct ether_header) + sizeof(struct ip)+ sizeof(udphdr);
                struct rtp_t* rtp = (struct rtp_t*)(res.data + offset);
                parse(*rtp);
                m_valid = true;
            }
        }
        return *this;
    }


    template<typename T>
    void RtpRFC::parse(T data)
    {
        m_fields.cc = (data.meta[0] << 4) & 0xf0;
        m_fields.v = (data.meta[0] >> 6);
        m_fields.p = (data.meta[0] >> 5) & 0x1;
        m_fields.x = (data.meta[0] >> 4) & 0x1;
        m_fields.m = (data.meta[1] >> 7);
        m_fields.pt = (data.meta[1]) & 0x7F;
        m_fields.timestamp = SWAP4(data.timestamp);
        m_fields.ssrc= SWAP4(data.SSRC);
        m_fields.ssrc= SWAP4(data.CSRC);
        jsonb.add(tjson::JsonField{"protocol", "RTP"});
        jsonb.add(tjson::JsonField{"CC", m_fields.cc});
        jsonb.add(tjson::JsonField{"ssrc", m_fields.ssrc});
        return; //just brek here
    }


}//rtp
