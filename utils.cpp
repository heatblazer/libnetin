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
        struct EthL4 eth;
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
                //TODO: resolve the tcp parsing also
                eth.type = EthL4::TCP;
            } else {
                //
                eth.type = EthL4::UNKNOWN;
            }
        }
        return eth;
    }
}
