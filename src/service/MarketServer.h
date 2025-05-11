#pragma once
#include "Market.h"
#include <functional>

class MarketServer {
    public:

    MarketServer(Market& m);
    
    Market& m;
    // Declare the function

    std::function<uint32_t(instrument::ticker)> getPrice = [this](instrument::ticker ticker){
         return m.getPrice(ticker);
    };

    std::function<void(instrument::ticker, instrument::ExternalOrder)> askOrder = [this](instrument::ticker ticker, instrument::ExternalOrder order){
        return m.placeAsk(ticker, instrument::createOrder(order)); 
    };
    
    std::function<void(instrument::ticker, instrument::ExternalOrder)> bidOrder = [this](instrument::ticker ticker, instrument::ExternalOrder order){
        return m.placeBid(ticker, instrument::createOrder(order)); 
    };

    int runServer();
};