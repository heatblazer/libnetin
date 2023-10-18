#include "rtp_t.h"
#include "utils.h"
#include <string.h>
#include <iostream>

namespace rtp {


RtpRFC::RtpRFC(const IParseable::type &res) : IParseable<Result_t,RtpRFC>{res}
    {
        memset(&m_fields, 0, sizeof(m_fields));
        memset(m_Payload, 0, sizeof(m_Payload)/sizeof(m_Payload[0]));
    }

    RtpRFC& RtpRFC::operator()()
    {
        return this->operator()(this->value);
    }

    RtpRFC &RtpRFC::operator()(const IParseable::type &res)
    {
        PRINTDBG("rtp_t.cpp");

        struct EthL4 eth = utils::GetEthL4(res.data);
        size_t total = res.out.len;

        switch (eth.type) {
            case EthL4::UDP: {
                int offset = sizeof(struct ether_header) + sizeof(struct ip)+ sizeof(udphdr);
                memcpy(m_Payload, res.data+offset,total-offset);
                struct rtp_t* rtp = (struct rtp_t*)(res.data + offset);
                parse(*rtp, eth);
            }
            default:
                break;
        }
        return *this;
    }


    void RtpRFC::parse(rtp_t data, const EthL4& res)
    {
        Valid = true;
        m_fields.cc = (data.meta[0] << 4) & 0xf0;
        m_fields.v =  (data.meta[0] >> 6);
        m_fields.p =  (data.meta[0] >> 5) & 0x1;
        m_fields.x =  (data.meta[0] >> 4) & 0x1;
        m_fields.m =  (data.meta[1] >> 7);
        m_fields.pt = (data.meta[1]) & 0x7F;
        m_fields.timestamp = SWAP4(data.timestamp);
        m_fields.ssrc= SWAP4(data.SSRC);
        m_fields.csrc= SWAP4(data.CSRC);
        if (m_fields.v <= 0 || m_fields.v > 2)
            Valid |= false;
        if (m_fields.pt <= 0 || m_fields.pt > 126)
            Valid |= false;
        if (Valid) {
            jsonb.add(tjson::JsonField{ "protocol", "RTP" });
            jsonb.add(tjson::JsonField{ "srcip", res.sourceIP });
            jsonb.add(tjson::JsonField{ "dstip", res.destIP });
            jsonb.add(tjson::JsonField{ "PT", m_fields.pt });
            jsonb.add(tjson::JsonField{ "CC", m_fields.cc });
            jsonb.add(tjson::JsonField{ "ssrc", m_fields.ssrc });
            jsonb.add(tjson::JsonField{ "csrc", m_fields.csrc });
            jsonb.add(tjson::JsonField{ "TS", m_fields.timestamp });
            jsonb.add(tjson::JsonField{ "EX", m_fields.x });
        }
        return; //just brek here
    }

}//rtp
