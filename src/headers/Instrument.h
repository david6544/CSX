#pragma once

#include <cinttypes>
#include <functional>
#include "rpc/this_handler.h"
#include "Utils.h"

namespace instrument {
    
    // Ticker names are stored as uint16_t and static_casted to Tickers defined in {Tickers.h}
    typedef uint16_t ticker;
    
    struct Order {
        uint64_t time;   
        uint32_t price;
        uint32_t quantity;
        MSGPACK_DEFINE(time, price, quantity);
    };

    struct ExternalOrder {
        uint32_t price;
        uint32_t quantity;
        MSGPACK_DEFINE(price, quantity);
    };


    static const instrument::Order createOrder(instrument::ExternalOrder externalOrder) {
        return {
            .time = utils::getCurrentTime(),
            .price = externalOrder.price,
            .quantity = externalOrder.quantity,
        };
    }

    static constexpr bool isOrderEqual(const Order & l, const Order & r) {
        if (l.time == r.time && l.price == r.price && l.quantity == r.quantity) {
            return true;
        }
        
        return false;
    }
        
    // Price/time order match for bids
    struct BidComparator {
        bool operator()(const Order& l, const Order &r) const {
            if (l.price != r.price) {
                return l.price < r.price; // Greater Price first
            }
            return l.time > r.time; 
        }
    };


    // Price/time order match for asks
    struct AskComparator {
        bool operator()(const Order& l, const Order&  r) const {
            if (l.price != r.price) {
                return l.price > r.price; // Lesser price first
            }
            return l.time > r.time; 
        }
    };

}
