#include "turn_t.h"
#include "stun_t.h"
#include "rtp_t.h"
#include "utils.h"

using namespace stun;
using namespace rtp;

namespace turn {


    TurnRFC::TurnRFC(const IParseable::type &ref)
        : IParseable<Result_t,TurnRFC>{ref}
    {
    }

    TurnRFC &TurnRFC::operator()(const IParseable::type &res)
    {
        PRINTDBG("turn_t.cpp");

        struct EthL4 eth = utils::GetEthL4(res.data);
        switch (eth.type) {
        case EthL4::UDP: {
                unsigned idx = (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(udphdr));
                unsigned short chann = (res.data[idx]) << 8 | (res.data[idx+1]);
                unsigned short len = (res.data[idx+2] << 8) | (res.data[idx+3]);
                if (chann == 0x8078) {
                    Valid = true;
                    jsonb.add(tjson::JsonField{"protocol", "TURN"});
                    jsonb.add(tjson::JsonField{"srcip", eth.sourceIP});
                    jsonb.add(tjson::JsonField{"dstip", eth.destIP});
                    jsonb.add(tjson::JsonField{"chan", chann});
                    jsonb.add(tjson::JsonField{"length", len});
                    RtpRFC rtp{res} ;
                    StunRFC stun{res};
                    if (stun().Valid) {
                        jsonb.add(tjson::JsonField{"nested-protocol", "STUN in TURN"});
                    }
                    if (rtp().Valid) {
                        jsonb.add(tjson::JsonField{"nested-protocol", "RTP in TURN"});
                    }

                }
        }
        default:
            break;
        }
        return  *this;
    }

    TurnRFC &TurnRFC::operator()()
    {
        return this->operator()(this->value);
    }

} //turn
