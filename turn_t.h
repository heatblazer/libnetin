#ifndef TURN_T_H
#define TURN_T_H

#include "types.h"
#include "tjson.hpp"


namespace turn {

    struct TurnRFC: public IParseable<Result_t,TurnRFC>
    {
        TurnRFC() = delete;

        TurnRFC(const IParseable::type& ref);

        TurnRFC& operator()(const IParseable::type&);

        TurnRFC& operator()();

        Result_t::TypeRFC type() const { return Result_t::TypeRFC::TURN;}


    };
}

#endif // TURN_T_H
