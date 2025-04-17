#include "Market.h"


Market::Market() {
    //do the uniqe ptr stuff here
    this->market = std::array<Market::books, marketSize>();
}

void Market::addTicker(instrument::ticker ticker) {
    //do unique ptr stuff here
    Orderbook<instrument::BidComparator> bidBook = Orderbook<instrument::BidComparator>();
    Orderbook<instrument::AskComparator> askBook = Orderbook<instrument::AskComparator>();
    Market::books tickerBooks = {
        bidBook,
        askBook,
    };
    if (ticker < market.size()) {
        market[ticker] = tickerBooks;
    }
}

bool Market::placeBid(instrument::ticker ticker, instrument::Order order) {
    Market::books& tickerBooks = this->market.at(ticker);
    tickerBooks.bids.placeOrder(order);

    instrument::Order bestBid = tickerBooks.bids.getBest();
    if (!instrument::isOrderEqual(bestBid, order) || tickerBooks.asks.isEmpty()) {
        return 0;
    };
    this->matchOrder(tickerBooks);
    return 1;
}

bool Market::placeAsk(instrument::ticker ticker, instrument::Order order) {
    Market::books& tickerBooks = this->market.at(ticker);
    tickerBooks.asks.placeOrder(order);

    instrument::Order bestAsk = tickerBooks.asks.getBest();
    if (!instrument::isOrderEqual(bestAsk, order) || tickerBooks.bids.isEmpty()) {
        return 0;
    };
    this->matchOrder(tickerBooks);
    return 1;
}

void Market::matchOrder(Market::books books) {
    instrument::Order bestAsk = books.asks.getBest();
    instrument::Order bestBid = books.bids.getBest();
    if (bestAsk.price <= bestBid.price) {
        std::cout << "match at: " << bestAsk.price << " " << bestBid.price << std::endl;
        uint32_t minQuantity = std::min(bestAsk.quantity, bestBid.quantity);
        bestAsk.quantity -= minQuantity;
        bestBid.quantity -= minQuantity;

        if (bestAsk.quantity == 0) {
            books.asks.clearBest();
        }
        if (bestBid.quantity == 0) {
            books.bids.clearBest();
        }
    }
} 

uint32_t Market::getPrice(instrument::ticker ticker) {
    Market::books books = this->market.at(ticker);
    
    uint32_t askPrice = books.asks.getBest().price;
    uint32_t bidPrice = books.bids.getBest().price;
    
    return (bidPrice + (askPrice - bidPrice) / 2);
}