#include "nill_t.h"
NullRFC &NullRFC::operator()()
{
    //jsonb.add(tjson::JsonField{"protocol", "NillRFC"});
    return *this;
}


NullRFC& NullRFC::operator()(const IParseable::type &res MAYBEUNUSED)
{
    return *this;
}
