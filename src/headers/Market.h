#pragma once
#include"Orderbook.h"
#include<array>
#include<thread>
#include<atomic>

class Market {

    private:
        static const uint16_t marketSize = 500;    
        std::thread executionThread;
        std::array<std::atomic<uint32_t>, marketSize> cachedPrices;
        
        void processOrdersLoop();
    public:    
    
        struct books {
            Orderbook<instrument::BidComparator> bids;
            Orderbook<instrument::AskComparator> asks;
        };

        std::array<Market::books, marketSize> market;
        
        void placeAsk(instrument::ticker ticker,const instrument::Order & order);
        void placeBid(instrument::ticker ticker,const instrument::Order & order);
        uint32_t getPrice(instrument::ticker ticker);
        
        void matchOrder(books& books, instrument::ticker ticker);
    
        void addTicker(instrument::ticker ticket);
        Market();
        
    private:
};