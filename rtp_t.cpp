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

    RtpRFC::RtpRFC()
    {

    }

    bool RtpRFC::valid() const
    {
        return true;
    }

    RtpRFC &RtpRFC::operator()(const Pcap::Result_t &res)
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
                char tmp[128]={0};
                memcpy(tmp, res.data+offset, sizeof(rtp_t));
                parse(*rtp);
            }
        }
        return *this;
    }


    template<typename T>
    void RtpRFC::parse(T data)
    {
        char tmp[sizeof(rtp_t)] = {0};
        memcpy(tmp, &data, sizeof(rtp_t));
#if 1
        //data.uv.val = SWAP4(data.uv.val);

        int cc = (data.meta[0] << 4) & 0xf0;
        int v = (data.meta[0] >> 6);
        int p = (data.meta[0] >> 5) & 0x1;
        int x = (data.meta[0] >> 4) & 0x1;

        data.timestamp = SWAP4(data.timestamp);
        data.SSRC = SWAP4(data.SSRC);
        data.CSRC = SWAP4(data.CSRC);


#endif
        return;
    }


}//rtp
