#pragma once

#include "Instrument.h"
#include "OrderQueue.h"

#include <queue>
#include <deque>
#include <iostream>
#include <memory>


template <typename T>// bid or ask comparator
class Orderbook {
    
    public:

    std::priority_queue
    <
        instrument::Order, // Type
        std::deque<instrument::Order>, // Continaer
        T // Comparator
    > orders;

    std::shared_ptr<LockFreeOrderQueue<instrument::Order>>orderQueue;

    Orderbook() {
        orderQueue = std::make_shared<LockFreeOrderQueue<instrument::Order>>(3000);
    }

    Orderbook(size_t messageCapacity) {
        orderQueue = std::make_shared<LockFreeOrderQueue<instrument::Order>>(messageCapacity);
    }
    
    void placeOrder(const instrument::Order& order) {
        this->orderQueue.get()->enqueue(order);
    }
    
    void executeOrder() {
        auto opt = this->orderQueue.get()->dequeue();
        if (opt) {
            auto order = std::move(*opt); // possibly slow? need to look into ove sematics for optionals
            orders.push(order);
        }
    }

    void clearBest()  {
        orders.pop();
    }

    bool isEmpty() {
        return orders.empty();
    }
    
    const instrument::Order& getBest() {
        return orders.top();
    }

    uint64_t getMarketDepth() {
        return orders.size();
    }
};