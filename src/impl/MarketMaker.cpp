#include "MarketMaker.h"


void MarketMaker::addLiquidity(Market& market, instrument::ticker ticker) {

    const int numOrders = 10; //could randomize this

    uint32_t price = market.getPrice(ticker);
    
    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> bidDist(price * (1 - spread / 2), price * stddev);
    std::normal_distribution<double> askDist(price * (1 - spread / 2), price * stddev);

    
    for (int i = 0; i < numOrders; ++i) {
        uint32_t bidPrice = static_cast<uint32_t>(bidDist(generator));
        uint32_t askPrice = static_cast<uint32_t>(askDist(generator));
    
        uint32_t quantity = 100; // could randomize this too

        uint32_t currTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        market.placeBid(ticker, instrument::Order{.price = bidPrice, .quantity = quantity, .time = currTime});
        market.placeAsk(ticker, instrument::Order{.price = askPrice, .quantity = quantity, .time = currTime});
    }

}