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

    StunRFC::StunRFC() : m_isValid{false},
    m_stunCnt{0},
    m_attribdata{0}
    {
        memset(&m_properties, 0, sizeof(m_properties));
    }

    StunRFC &StunRFC::operator ()(const Pcap::Result_t & res)
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
                struct stun_t* pStun = (struct stun_t*)(res.data + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(udphdr));
                if (pStun)
                {
                    parsemessage((const char*)pStun);
                }
                m_properties.sourcePort = ntohs(udpHeader->source);
                m_properties.dstPort = ntohs(udpHeader->dest);
            }
        }
        return  *this;
    }

    bool StunRFC::valid() const
    {
        return  m_isValid;
    }

    size_t StunRFC::count() const
    {
        return m_stunCnt;
    }

    AppName StunRFC::app()
    {
        AppName n = Unknown;
        if (m_properties.tokens >= 2) {
            n = AppName::WhatsApp;
            return n;
        } else {
            switch (m_properties.priority) {
            case 1862269950:
            case 1862269438:
            case 1862270462: {
                n = AppName::Skype;
                break;
            }
            case 1845501695: {
                n = AppName::Facebook;
                break;
            }
            case 1853824768: {
                n = AppName::Telegram;
                break;
            }
            case 1853824767: {
                n = AppName::GoogleMeet;
                break;
            }
            default://zoom and other
                break;
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
            m_isValid = true;
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
