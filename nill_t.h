#ifndef NILL_T_H
#define NILL_T_H
#include "types.h"
#include "tjson.hpp"

struct NullRFC : public IParseable<Result_t>
{
    bool valid() const { return false; }

    NullRFC& operator()();

    Result_t::TypeRFC type() const { return Result_t::TypeRFC::RTP;}

};

#endif // NILL_T_H
