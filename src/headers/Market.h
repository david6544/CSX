
#include"Orderbook.h"
#include<array>

class Market {

    private:
        static const uint16_t marketSize = 500;
    public:


        struct books {
            Orderbook<instrument::BidComparator> bids;
            Orderbook<instrument::AskComparator> asks;
        };

        std::array<Market::books, marketSize> market;
        
        bool placeAsk(instrument::ticker ticker, instrument::Order order);
        bool placeBid(instrument::ticker ticker, instrument::Order order);
        
        void matchOrder(books);
    
        void addTicker(instrument::ticker ticket);
        Market();
        
    private:
};