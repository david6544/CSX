#include "Instrument.h"
#include <queue>
#include <deque>
#include <iostream>


template <typename T>// bid or ask comparator
class Orderbook {
    
    public:

    std::priority_queue
    <
        instrument::Order, // Type
        std::deque<instrument::Order>, // Continaer
        T // Comparator
    > orders;

    Orderbook() {}
    
    void placeOrder(const instrument::Order& order) {
        orders.push(order);
    }

    void clearBest() {
        orders.pop();
    }

    bool isEmpty() {
        return orders.empty();
    }
    
    const instrument::Order& getBest() const {
        return orders.top();
    }

    uint64_t getMarketDepth() const {
        return orders.size();
    }
};