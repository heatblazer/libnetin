#ifndef WEBSOCKET_T_H
#define WEBSOCKET_T_H
#include "defs.h"
#include "types.h"
#include "tjson.hpp"

namespace websocket {

    struct WebSocketRFC: public IParseable<Result_t, WebSocketRFC>
    {
        enum
        {
            StartHeader = 0x82U,

            PayloadLenMask = 0x7FU,

            MaskOffset = 2U,

            PayloadOffset = 6U,

            MaskByteSize = 4U,
            
            Unknown = 0// convinience

        } eWebSocketConsts;


        unsigned char m_maskKey[4];
    public:
        WebSocketRFC() = delete;

        ~WebSocketRFC() noexcept ;

        explicit WebSocketRFC(const IParseable::type& res);

        WebSocketRFC &operator()(const IParseable::type &res);

        WebSocketRFC& operator()();

        inline Result_t::TypeRFC type() const { return Result_t::TypeRFC::WebSocket;}
    };

} //ns websocket


#endif // WEBSOCKET_T_H
