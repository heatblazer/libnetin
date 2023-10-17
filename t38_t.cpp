#include "defs.h"
#include "t38_t.h"
#include "utils.h"
#include <cstring>
#include <vector>
#include <unordered_map>

#include <cstdio>

namespace  {


    int wrbin(const char* f, const unsigned char* data, size_t dsize)
    {

        FILE* fp = fopen(f, "wb+");
        if (!fp) {
            return -1;
        }

        size_t ret = fwrite(data, sizeof(char), dsize, fp);
        fclose(fp);
        return  ret;
    }

}

struct MPair{
    std::vector <unsigned char> hdlcdata;
    std::vector <unsigned char> sigdata;
};

using Map2D = std::unordered_map<std::string, MPair>;

namespace t38
{

static Map2D s_T38Flows;

int T38Rfc::state(MAYBEUNUSED const struct EthL4 &e4hdr, size_t len)
{
    unsigned short seq = m_Payload[0] << 8 | m_Payload[1];
    unsigned short typeofmsg = m_Payload[2] << 8 | m_Payload[3];

    if (seq) {
//        std::cout << "SEQ: " <<seq << "\r\n";
    }
    switch (typeofmsg) {
        case 0x0106: {
            break;
        }
        case 0x0104: {
            break;
        }
        case 0x06c0: {
            // begin collect data
            s_T38Flows[e4hdr.sourceIP].sigdata.push_back(m_Payload[8]);
            break;
        }
        case 0x03c0: {
            //sig end
            break;
        }
        FALLTROUGH;
        case 0x0ed0:
        case 0x4dd0: { // data trans
            for(size_t i=4; i < len; i++) {
                s_T38Flows[e4hdr.sourceIP].sigdata.push_back(m_Payload[i]);
            }
            break;
        }
        FALLTROUGH;
        case 0x00:
        default: {
            break;
        }
    }
    return 0;

}

T38Rfc::T38Rfc(const IParseable::type &res) : IParseable<Result_t,T38Rfc>{res}
    {
        memset(m_Payload, 0, sizeof(m_Payload)/sizeof(m_Payload[0]));
    }

    T38Rfc &T38Rfc::operator()(MAYBEUNUSED const IParseable::type &res)
    {

        struct EthL4 eth = utils::GetEthL4(res.data);
//bla        size_t total = res.out.len;
//1        eth.destIP;
//2        eth.sourceIP;
        switch (eth.type) {
        case EthL4::UDP: {
            int offset = sizeof(struct ether_header) + sizeof(struct ip) + sizeof(udphdr);
//            struct udphdr* udptl = (struct udphdr*) (res.data + offset );
//            unsigned short ln = (udptl->len);
//            unsigned short dst = (udptl->dest);
//            unsigned short src = (udptl->source);
            memcpy(m_Payload, res.data+offset, res.out.caplen-offset);//+offset,total-offset);
//            struct rtp_t* rtp = (struct rtp_t*)(res.data + offset);
//            jsonb.add(tjson::JsonField{"protocol", "RTP"});
//            jsonb.add(tjson::JsonField{"srcip", eth.sourceIP});
//            jsonb.add(tjson::JsonField{"dstip", eth.destIP});

            state(eth, res.out.caplen-offset);

            break;
        }
        case EthL4::TCP: {
            break; //mmm
        }
        default:
            break;
        }
        return *this;
    }

    void T38Rfc::dbg()
    {



        for (const auto &kv : s_T38Flows) {
            std::cout << kv.first << std::endl;
            size_t faxdatas = wrbin(kv.first.c_str(), &kv.second.sigdata[0], kv.second.sigdata.size());

            std::cout << "Written:" << faxdatas << std::endl;


 //           for (auto iit : kv.second.sigdata) {

//                std::cout << "[" << iit << "]";
 //           }
        }
    }

    T38Rfc &T38Rfc::operator()()
    {
        return this->operator()(this->value);
    }
};
