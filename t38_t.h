#ifndef T38_T_H
#define T38_T_H
#include "defs.h"
#include "types.h"
#include "tjson.hpp"
#include <pcap/pcap.h>


namespace t38
{


    struct t38_t
    {
    };


    class T38Rfc :  public IParseable<Result_t, T38Rfc>
    {
        unsigned char m_Payload[1500];

        int state(const EthL4 &e4hdr, size_t len);


    public:
        T38Rfc() = delete;

        T38Rfc(const IParseable::type& res);

        T38Rfc& operator()(const IParseable::type &res);

        T38Rfc& operator()();

        inline Result_t::TypeRFC type() const { return Result_t::TypeRFC::T38;}


        static void dbg();
    };


}//t38

#endif // T38_T_H
