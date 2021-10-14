#ifndef STUN_T_H
#define STUN_T_H
#include "defs.h"
#include "Pcap.h"
#include <pcap/pcap.h>
#include <vector>
#include <unordered_map>
#define MAX_BUFF_SIZE 1500



namespace stun {

    enum AppName
    {
        WhatsApp,
        Facebook,
        Telegram,
        Skype,
        Zoom, // not stun
        GoogleMeet,
        Unknown,
        Size = Unknown
    };

    /**
     * @brief The MessageTypes message types of STUN session
     */
    enum MessageTypes
    {
        BINDING_REQUEST = 0x0001,
        BINDING_RESPONSE = 0x0101,
        SEND_INDICATION = 0x0016,
        CREATE_PERMISSION = 0x0008,
        BINDING_ERROR_RESPONSE = 0x0111,
        CREATE_PERM_ERR_RESPONSE = 0x0118,
        SHARED_SECRET_REQUEST = 0x0002,
        SHARED_SECRET_RESPONSE = 0x0102,
        SHARED_SECRET_ERROR_RESPONSE = 0x0112,
        ALLOCATE_REQUEST = 0x0003,
        ALLOCATE_REQUEST_ERROR_RESPONSE = 0x0113,
        FILLIN = 0x16FE,
        ALLOCATE_SUCCESS_RESPONSE = 0x0103,
        REFRESH_REQUEST = 0x0004,
        CHANNEL_BIND_REQUEST = 0x0009,
        DATA_INDICATION = 0x0017,
        CREATE_PERMISSION_SUCCESS_RESPONSE = 0x0108,
        CHANEL_BIND_SUCCESS_RESPONSE = 0x0109,
        REFRESH_ERROR_RESPONSE = 0x0114
    };

    /**
     * @brief The MessageAttribs describes stun attributes
     */
    enum MessageAttribs
    {
        MAPPED_ADDRESS = 0x0001,
        RESPONSE_ADDRESS = 0x0002,
        CHANGE_REQUEST = 0x0003,
        SOURCE_ADDRESS = 0x0004,
        CHANGED_ADDRESS = 0x0005,
        USERNAME = 0x0006,
        PASSWORD = 0x0007,
        MESSAGE_INTEGRITY = 0x0008,
        ERROR_CODE = 0x0009,
        UNKNOWN_ATTRIBUTES = 0x4000,
        REFLECTED_FROM = 0x000b,
        XOR_PEER_ADDRESS = 0x0012,
        XOR_RELAYED_ADDRESS = 0x0016,
        DATA = 0x0013,
        ICE_CONTROLED = 0x8029,
        ICE_CONTROLLING = 0x802a,
        XOR_MAPPED = 0x0020,
        UNKNOWN = 0xc057,
        SOFTWARE = 0x8022,
        PRIORITY = 0x0024,
        RESERVATION_TOKEN = 0x0022,
        DEFAULT = 0xFFFF
    };

    /**
     * @brief The stun_t struct
     * descirbes the well known STUN header
     */
    struct stun_t
    {
        unsigned short message_type;
        unsigned short message_len;
        unsigned int magic_cookie;
        unsigned char transaction_id[12];
    };

    /**
     * @brief The AttribDataPair aggregates the attribute data from the stun header
     * shall be put in a list
     */
    struct AttribDataPair
    {
        explicit AttribDataPair(const MessageAttribs& attr, const u_char* data, const size_t len);
        MessageAttribs attribute;
        std::vector<u_char> data;
    };

    /**
     * @brief The StunAttribute aggregation of STUN attributes.
     */
    struct StunAttribute
    {
        StunAttribute();
        ~StunAttribute();
        std::vector<AttribDataPair> m_attribs;
    };

    /**
     * @brief The StunRFC construction of the whole stun RFC callable
     */
    struct StunRFC: public IParseable<Result_t,StunRFC>
    {
        StunRFC();

        StunRFC(const IParseable::type& ref);

        StunRFC& operator()(const IParseable::type&);

        StunRFC& operator()();

        Result_t::TypeRFC type() const { return Result_t::TypeRFC::STUN;}

        size_t count() const;

        AppName app();


        static std::unordered_map<std::string, int> s_UsernamesHist;
    private:

        void parsemessage(const char* data);

        void parseattribs(const unsigned char* data, const size_t len);

        size_t m_stunCnt;
        MessageTypes m_type;
        StunAttribute m_attributes;
        //unsigned char m_attribdata[MAX_BUFF_SIZE];
        struct {
            unsigned short sourcePort;
            unsigned short dstPort;
            unsigned int tokens;        //count tokens here
            unsigned int priority;
            bool nested;
        } m_properties;

    };

}//stun


#endif // STUN_T_H
