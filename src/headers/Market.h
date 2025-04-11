
#include "Orderbook.h"
#include<unordered_map>

class Market {
   
    public:

        struct books {
            Orderbook<instrument::BidOrder> bids;
            Orderbook<instrument::AskOrder> asks;
        };
        
        std::unordered_map<instrument::Ticker,Market::books> market;

        bool placeAsk(instrument::Ticker ticker, instrument::Order order);
        bool placeBid(instrument::Ticker ticker, instrument::Order order);
   
        static constexpr void matchOrder(Market::books books);
    private:
};