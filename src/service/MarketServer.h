#pragma once
#include "Market.h"
#include <functional>

class MarketServer {
    public:

    MarketServer(Market& m);
    
    Market& m;
    // Declare the function

    std::function<uint32_t(instrument::ticker)> getPrice = [this](instrument::ticker ticker){ return m.getPrice(ticker); };
    std::function<bool(instrument::ticker, instrument::Order)> askOrder = [this](instrument::ticker ticker, instrument::Order order){ return m.placeAsk(ticker, order); };
    std::function<bool(instrument::ticker, instrument::Order)> bidOrder = [this](instrument::ticker ticker, instrument::Order order){ return m.placeBid(ticker, order); };

    int runServer();
};