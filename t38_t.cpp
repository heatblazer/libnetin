#include "defs.h"
#include "t38_t.h"

namespace t38
{


    T38Rfc::T38Rfc(const IParseable::type &res) : IParseable<Result_t,T38Rfc>{res}
    {

    }

    T38Rfc &T38Rfc::operator()(MAYBEUNUSED const IParseable::type &res)
    {
        return *this;
    }

    T38Rfc &T38Rfc::operator()()
    {
        return this->operator()(this->value);
    }

};
