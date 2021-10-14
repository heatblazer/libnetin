#ifndef NILL_T_H
#define NILL_T_H
#include "types.h"
#include "tjson.hpp"

struct NullRFC : public IParseable<Result_t, NullRFC>
{

    int m_dummyData[10];

    NullRFC& operator()();

    Result_t::TypeRFC type() const { return Result_t::TypeRFC::RTP;}

    NullRFC() : IParseable<Result_t, NullRFC>{}
    {
        //tst
    }

};

#endif // NILL_T_H
