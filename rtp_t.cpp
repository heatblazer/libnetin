#include "rtp_t.h"
#include "utils.h"

#include <string.h>

#include <iostream>

namespace rtp {

    RtpRFC::RtpRFC()
    {

    }

    RtpRFC::RtpRFC(const IParseable::type &res) : IParseable<Result_t>{res}
    {

    }

    RtpRFC& RtpRFC::operator()()
    {
        return this->operator()(this->value);
    }

    RtpRFC &RtpRFC::operator()(const IParseable::type &res)
    {
        struct EthL4 eth = utils::GetEthL4(res.data);
        switch (eth.type) {
            case EthL4::UDP: {
                int offset = sizeof(struct ether_header) + sizeof(struct ip)+ sizeof(udphdr);
                struct rtp_t* rtp = (struct rtp_t*)(res.data + offset);
                jsonb.add(tjson::JsonField{"protocol", "RTP"});
                jsonb.add(tjson::JsonField{"srcip", eth.sourceIP});
                jsonb.add(tjson::JsonField{"dstip", eth.destIP});
                parse(*rtp);
                Valid = true;
            }
            default:
                break;
        }
        return *this;
    }


    template<typename T>
    void RtpRFC::parse(T data)
    {
        m_fields.cc = (data.meta[0] << 4) & 0xf0;
        m_fields.v =  (data.meta[0] >> 6);
        m_fields.p =  (data.meta[0] >> 5) & 0x1;
        m_fields.x =  (data.meta[0] >> 4) & 0x1;
        m_fields.m =  (data.meta[1] >> 7);
        m_fields.pt = (data.meta[1]) & 0x7F;
        m_fields.timestamp = SWAP4(data.timestamp);
        m_fields.ssrc= SWAP4(data.SSRC);
        m_fields.csrc= SWAP4(data.CSRC);
        jsonb.add(tjson::JsonField{"CC", m_fields.cc});
        jsonb.add(tjson::JsonField{"ssrc", m_fields.ssrc});
        jsonb.add(tjson::JsonField{"csrc", m_fields.csrc});
        jsonb.add(tjson::JsonField{"timestamp", m_fields.timestamp});
        jsonb.add(tjson::JsonField{"extension", m_fields.x});

        return; //just brek here
    }


}//rtp
