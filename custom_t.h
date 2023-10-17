#ifndef CUSTOM_T_H
#define CUSTOM_T_H
#include "defs.h"
#include "types.h"
#include "tjson.hpp"

namespace custom {

struct CustomRFC : public IParseable<Result_t, CustomRFC>
{

    CustomRFC& operator()();

    CustomRFC& operator()(const IParseable::type&);

    Result_t::TypeRFC type() const { return Result_t::TypeRFC::CUSTOM;}

    CustomRFC(const IParseable::type& res MAYBEUNUSED) ;

};

} // custom

#endif // CUSTOM_T_H
