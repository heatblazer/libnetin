#ifndef NILL_T_H
#define NILL_T_H
#include "types.h"
#include "tjson.hpp"

/**
 * @brief The NullRFC test device class - does nothing
 * but can be used for mockups and custom tests
 */
struct NullRFC : public IParseable<Result_t, NullRFC>
{

    int m_dummyData[10];

    NullRFC& operator()();

    Result_t::TypeRFC type() const { return Result_t::TypeRFC::UNKNOWN;}

    NullRFC(const IParseable::type& res MAYBEUNUSED) : IParseable<Result_t, NullRFC>{res}
    {
        //tst
    }

};

#endif // NILL_T_H
