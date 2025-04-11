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

    
    Orderbook();

    instrument::Order getBest();
    void placeOrder(instrument::Order order);

    uint64_t getMarketDepth();
};