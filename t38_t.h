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
    public:
        T38Rfc() = default;

        T38Rfc(const IParseable::type& res);

        T38Rfc& operator()(const IParseable::type &res);

        T38Rfc& operator()();

        Result_t::TypeRFC type() const { return Result_t::TypeRFC::T38;}

    };


}//t38

#endif // T38_T_H
