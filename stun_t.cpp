#include "stun_t.h"
#include "utils.h"

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


#define STUN_MAGIC_COOKIE 0x2112A442

namespace stun {

    StunRFC::StunRFC() :
    m_stunCnt{0},
    m_attribdata{0}
    {
        memset(&m_properties, 0, sizeof(m_properties));
    }

    StunRFC::StunRFC(const IParseable::type &ref)
        : IParseable<Result_t>{ref},
        m_stunCnt{0}, m_attribdata{0}
    {
        memset(&m_properties, 0, sizeof(m_properties));
    }

    StunRFC &StunRFC::operator ()()
    {
        return this->operator()(this->value);
    }


    StunRFC &StunRFC::operator ()(const IParseable::type & res)
    {
        struct EthL4 eth = utils::GetEthL4(res.data);
        switch (eth.type) {
        case EthL4::UDP: {
            struct stun_t* pStun = (struct stun_t*)(res.data + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(udphdr));            
            parsemessage((const char*)pStun);
            jsonb.add(tjson::JsonField{"srcip", eth.sourceIP});
            jsonb.add(tjson::JsonField{"dstip", eth.destIP});
            m_properties.sourcePort = ntohs(eth.udpHeader->source);
            m_properties.dstPort = ntohs(eth.udpHeader->dest);
            auto a = app();
            (void)a;
        }
        default:
            break;
        }
        return  *this;
    }

    size_t StunRFC::count() const
    {
        return m_stunCnt;
    }

    AppName StunRFC::app()
    {
        jsonb.add(tjson::JsonField{"tokens", m_properties.tokens});

        AppName n = Unknown;
        if (m_properties.tokens >= 2) {
            n = AppName::WhatsApp;
            jsonb.add(tjson::JsonField{"app", "WhatsApp"});
            return n;
        } else {
            switch (m_properties.priority) {
            case 1862269950:
            case 1862269438:
            case 1862270462: {
                n = AppName::Skype;
                jsonb.add(tjson::JsonField{"app", "Skype"});
                break;
            }
            case 1845501695: {
                n = AppName::Facebook;
                jsonb.add(tjson::JsonField{"app", "Facebook"});
                break;
            }
            case 1853824768: {
                n = AppName::Telegram;
                jsonb.add(tjson::JsonField{"app", "Telegram"});
                break;
            }
            case 1853824767: {
                n = AppName::GoogleMeet;
                jsonb.add(tjson::JsonField{"app", "GoogleMeet"});
                break;
            }
            default: {
                jsonb.add(tjson::JsonField{"app", "Unknown"});
                break;
                }
            }            
        }
        return n;
    }

    void StunRFC::parsemessage(const char *data)
    {
        struct stun_t* pStun = (struct stun_t*)(data);
        if (pStun) {
            unsigned int cookie = SWAP4(pStun->magic_cookie);
            unsigned short messagetype = SWAP2(pStun->message_type);
            unsigned short messagelen = SWAP2(pStun->message_len);
            if (cookie != STUN_MAGIC_COOKIE) {
                return;
            }
            if (!m_properties.nested)
                jsonb.add(tjson::JsonField{"protocol", "STUN"});
            else
                jsonb.add(tjson::JsonField{"protocol-nested", "STUN"});
            Valid = true;
            m_type = (MessageTypes) messagetype;
            if (messagelen > 0) {
                pStun++;
                memcpy(m_attribdata, pStun,messagelen);
                parseattribs(messagelen);
            }
            m_stunCnt++;
        }
    }


    void StunRFC::parseattribs(const size_t len)
    {

        for(size_t i=0; i < len;) {
            MessageAttribs attr  = (MessageAttribs) ((m_attribdata[i] << 8) | (m_attribdata[i+1]));
            unsigned short attrlen = (m_attribdata[i+2] << 8) | (m_attribdata[i+3]);            
             //reserved 4 bytes, move to the actual data
            i+=4;
            // pad by 4 since stun specifies so
            i+= utils::padcalc<4>(attrlen);
            switch (attr) {
                case UNKNOWN_ATTRIBUTES: {
                    break;
                }
                case MAPPED_ADDRESS:
                case RESPONSE_ADDRESS:
                case CHANGE_REQUEST:
                case SOURCE_ADDRESS:
                case CHANGED_ADDRESS:
                case XOR_PEER_ADDRESS: // todo xor peer address
                {
                    /*
                     * xport = int ((a[1][2] << 8) | a[1][3])
                    xport = (MAGIC_COOKIE >> 16) ^ xport
                    ip_orig = a[1][4] << 24 | a[1][5] << 16 | a[1][6] << 8 | a[1][7]
                    ip_orig ^= MAGIC_COOKIE
                    ip_xtype = [(ip_orig >> 24) & 0xFF, (ip_orig >> 16) & 0xFF, (ip_orig >> 8) & 0xFF, ip_orig & 0xFF]
                     * */
                    //0000   00 12 00 08 00 01 d4 d8 2b da c8 b6               ........+...
                    unsigned long long  xorpeer = utils::tobin<unsigned long long>((const char*)(&m_attribdata[i]));
                    break;
                }
                case SOFTWARE:
                    break;
                case ICE_CONTROLED:
                    break;
                case ICE_CONTROLLING:
                    break;
                case PRIORITY:
                {
                    m_properties.priority =utils::tobin<unsigned int>((const char*)(&m_attribdata[i]));
                    break;
                }
                case RESERVATION_TOKEN: {
                    m_properties.tokens += 1;
                    break;
                }
                case DATA:{
                    stun_t* pstn = (stun_t*)(m_attribdata+i);
                    unsigned int cookie = SWAP4(pstn->magic_cookie);
                    if (cookie == STUN_MAGIC_COOKIE) {
                        m_properties.nested =true;
                        parsemessage((const char*)pstn);
                    }
                    break;
                }
                case USERNAME: {
                    m_attributes.m_attribs.push_back(AttribDataPair((MessageAttribs)attr, m_attribdata+i, attrlen));
                    break;
                    }
                default:
                    break;
            }
            i+=attrlen; //move to the next attribute
        }
    }

    StunAttribute::StunAttribute()
    {
    }

    StunAttribute::~StunAttribute()
    {

    }

    AttribDataPair::AttribDataPair(const MessageAttribs &attr, const u_char *data, const size_t len)
        : attribute{attr}
    {
        for (size_t i=0; i < len; i++)
            this->data.push_back(data[i]);
    }

} // stun
