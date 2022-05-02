#include "websocket_t.h"

namespace websocket {

WebSocketRFC::~WebSocketRFC() noexcept
{

}

WebSocketRFC::WebSocketRFC(const IParseable::type &res)
: IParseable<Result_t,WebSocketRFC>{res}
{

}

WebSocketRFC &WebSocketRFC::operator()(const IParseable::type &res MAYBEUNUSED)
{
// do parse here...
    return *this;
}


WebSocketRFC& WebSocketRFC ::operator()()
{
    return this->operator()(this->value);
}



} //ns websocket
