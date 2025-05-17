#pragma once

#include "rpc/this_handler.h"

namespace endpoints {

// Add this class member
enum class MessageType {
    Unknown,
    OrderSubmit,
    PriceRequest,
    MarketData,
};

struct PriceUpdate {
    uint16_t ticker;
    uint32_t price;
    MSGPACK_DEFINE(ticker, price);
};

}