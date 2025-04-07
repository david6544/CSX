#include "Instrument.h"
#include <queue>
#include <deque>


class Orderbook {

    private:
    

    instrument::Instrument instrument;
    
    
    public:
    std::priority_queue
    <
        instrument::Order, // Type
        std::deque<instrument::Order>, // Continaer
        instrument::OrderMatch // Comparator
    > orders;

    
    Orderbook();

    void placeOrder(instrument::Order order);
};