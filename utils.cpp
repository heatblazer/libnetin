#include "utils.h"
#include <string.h>


namespace utils {

    void split(const char *str, const char *delim, std::vector<std::string>& out)
    {
        const char* begin = str;
        const char* it = strstr(str, delim);
        if (it != NULL){
            std::string data{begin, it};
            out.push_back(data);
            it++;
            split(it, delim, out);
        } else {
            std::string data{str};
            out.push_back(data);
        }
    }


    struct EthL4 GetEthL4(const u_char* data)
    {

        #ifdef __unix__
        #define DATAOFFSET eth.tcpHeader->doff
        #else
        #define DATAOFFSET eth.tcpHeader->data_offset
        #endif

        struct EthL4 eth;
        memset(&eth, 0, sizeof(struct EthL4));
        eth.type = EthL4::UNKNOWN; // default
        if (!data)
            return eth;
        eth.ethernetHeader = (struct ether_header*)data;
        if (ntohs(eth.ethernetHeader->ether_type) == ETHERTYPE_IP) {
            eth.ipHeader = (struct ip*)(data + sizeof(struct ether_header));
            inet_ntop(AF_INET, &(eth.ipHeader->ip_src), eth.sourceIP, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(eth.ipHeader->ip_dst), eth.destIP, INET_ADDRSTRLEN);

            if (eth.ipHeader->ip_p == IPPROTO_UDP) {
                eth.udpHeader = (struct udphdr*)(data + sizeof(struct ether_header) + sizeof(struct ip));
                eth.type = EthL4::UDP;
            } else if (eth.ipHeader->ip_p == IPPROTO_TCP) {
                size_t total = sizeof(struct ether_header) + sizeof(struct ip);
                eth.tcpHeader = (struct tcphdr*)(data + total);
                if (DATAOFFSET > 5) {
                    total += sizeof(struct tcphdr);
                    eth.options = (unsigned char*) (data + total);
                    if (eth.options) {
                        eth.options_len = 2 + (unsigned int)eth.options[3];
                    }
                }
                eth.payload_len = eth.options_len + total;
                eth.type = EthL4::TCP;
            } else {
                //
                eth.type = EthL4::UNKNOWN;
            }
        }
        #undef DATAOFFSET
        return eth;
    }
}
