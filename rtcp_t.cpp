#include "rtcp_t.h"
#include "utils.h"
#include <string.h> //memset

namespace rtcp {

    /*
       |V=2|P|    RC   |   PT=SR=200   |             length            |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                         SSRC of sender                        |
       +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
sender |              NTP timestamp, most significant word             |
info   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |             NTP timestamp, least significant word             |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                         RTP timestamp                         |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                     sender's packet count                     |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                      sender's octet count                     |
       +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
report |                 SSRC_1 (SSRC of first source)                 |
block  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  1    | fraction lost |       cumulative number of packets lost       |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |           extended highest sequence number received           |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                      interarrival jitter                      |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                         last SR (LSR)                         |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                   delay since last SR (DLSR)                  |
       +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
report |                 SSRC_2 (SSRC of second source)                |
block  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  2    :                               ...                             :
       +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
       |                  profile-specific extensions                  |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/



    RtcpRFC::RtcpRFC(const IParseable::type &res)
        : IParseable<Result_t,RtcpRFC>{res}
    {
        memset(&m_fields, 0, sizeof(m_fields));
    }

    RtcpRFC &RtcpRFC::operator()(MAYBEUNUSED const IParseable::type &res)
    {

        struct EthL4 eth = utils::GetEthL4(res.data);
        switch (eth.type) {
        case EthL4::UDP: {
            MAYBEUNUSED uint16_t updDataLen = SWAP2(eth.udpHeader->len);

            int offset = sizeof(struct ether_header) + sizeof(struct ip) + sizeof(udphdr);
            struct rtcp_t* rtcp = (struct rtcp_t*)(res.data + offset);
                        
            uint8_t ver = rtcp->meta[0] >> 6;
            MAYBEUNUSED uint8_t p =  rtcp->meta[0] & 0x20;
            uint8_t rc = ((rtcp->meta[0] << 3) & 0xFF) >> 3;
            uint8_t pt = rtcp->meta[1] & 0xFF;
            uint16_t len = (rtcp->meta[2] << 8) | (rtcp->meta[3]);
            uint32_t ssrc = rtcp->SSRC_or_sender;

            if (ver == 2 && (pt >=  200 && pt <= 204 )) {
                jsonb.add(tjson::JsonField{ "protocol", "RTCP" });
                jsonb.add(tjson::JsonField{ "srcip", eth.sourceIP });
                jsonb.add(tjson::JsonField{ "dstip", eth.destIP });
                jsonb.add(tjson::JsonField{ "V", ver });
                jsonb.add(tjson::JsonField{ "PT", pt });
                jsonb.add(tjson::JsonField{ "ssrc", ssrc });
                jsonb.add(tjson::JsonField{ "Length", len});
                jsonb.add(tjson::JsonField{ "RC", rc});
                Valid = true;
            }
            //extend if needed 
        }
        default:
            break;
        }
        return *this;
    }

    RtcpRFC &RtcpRFC::operator()()
    {
        return this->operator()(this->value);
    }



}
