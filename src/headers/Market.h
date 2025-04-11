
#include"Orderbook.h"
#include<unordered_map>

class Market {
   
    public:

        struct books {
            Orderbook<instrument::BidComparator> bids;
            Orderbook<instrument::AskComparator> asks;
        };

        std::unordered_map<instrument::Ticker,Market::books, instrument::TickerHash> market;
        
        bool placeAsk(instrument::Ticker ticker, instrument::Order order);
        bool placeBid(instrument::Ticker ticker, instrument::Order order);
        
        void matchOrder(books);
    
        void addTicker(instrument::Ticker ticket);
        Market();
        
    private:
};