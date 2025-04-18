#include "MarketMaker.h"


void MarketMaker::addLiquidity(Market& market, instrument::ticker ticker) {

    const int numOrders = 1000; //could randomize this

    //limitation is that it doesn't adjust very well to incramental changes in price
    //Could be implemented with a historial high for the past x amount of trades etc.
    

    uint32_t newPrice = market.getPrice(ticker);
    if (newPrice != 0) {
        price = price * 0.9 + newPrice * 0.1; // Blend old/new prices
    }

    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> bidDist(price * (1 - spread / 2), price * stddev);
    std::normal_distribution<double> askDist(price * (1 + spread / 2), price * stddev);

    
    for (int i = 0; i < numOrders; ++i) {
        uint32_t bidPrice = static_cast<uint32_t>(bidDist(generator));
        uint32_t askPrice = static_cast<uint32_t>(askDist(generator));
    
        uint32_t quantity = 30; // could randomize this too

        uint32_t currTime = utils::getCurrentTime();
        market.placeBid(ticker, instrument::Order{.price = bidPrice, .quantity = quantity, .time = currTime});
        market.placeAsk(ticker, instrument::Order{.price = askPrice, .quantity = quantity, .time = currTime});
    }

}

void MarketMaker::setPrice(uint32_t price) {
    this->price = price;
}