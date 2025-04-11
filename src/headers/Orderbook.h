#include "Instrument.h"
#include <queue>
#include <deque>


template <typename T>// bid or ask comparator
class Orderbook {
    
    private:
    instrument::Ticker instrument;
    
    
    public:
    


    std::priority_queue
    <
        instrument::Order, // Type
        std::deque<instrument::Order>, // Continaer
        T // Comparator
    > orders;

    Orderbook() {
        orders = std::priority_queue<instrument::Order, std::deque<instrument::Order>, T>();
    }
    
    void placeOrder(instrument::Order order) {
        orders.push(order);
        return;
    }
    
    instrument::Order getBest() {
        return orders.top();
    }

    uint64_t getMarketDepth() {
        return orders.size();
    }
};