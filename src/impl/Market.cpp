#include "Market.h"
#include<thread>


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

bool Market::placeBid(instrument::ticker ticker, const instrument::Order & order) {

    Market::books& tickerBooks = this->market.at(ticker);
    tickerBooks.bids.placeOrder(order);

    const instrument::Order& bestBid = tickerBooks.bids.getBest();
    if (!instrument::isOrderEqual(bestBid, order) || tickerBooks.asks.isEmpty()) {
        return 0;
    };
    this->matchOrder(tickerBooks);
    return 1;
}

bool Market::placeAsk(instrument::ticker ticker, const instrument::Order& order) {

    Market::books& tickerBooks = this->market.at(ticker);
    tickerBooks.asks.placeOrder(order);

    const instrument::Order& bestAsk = tickerBooks.asks.getBest();
    if (!instrument::isOrderEqual(bestAsk, order) || tickerBooks.bids.isEmpty()) {
        return 0;
    };
    this->matchOrder(tickerBooks);
    return 1;
}
void Market::matchOrder(Market::books& books) {

    if (books.asks.isEmpty() || books.bids.isEmpty()) {
        return;
    }
    
    const instrument::Order& bestAsk = books.asks.getBest();
    const instrument::Order& bestBid = books.bids.getBest();
    
    while (bestAsk.price <= bestBid.price && (!books.asks.isEmpty() && !books.bids.isEmpty())) {
        std::cout << "match at: " << std::endl;
        
        std::cout << bestAsk.price << ": " << bestAsk.quantity << std::endl;
        std::cout << bestBid.price << ": " << bestBid.quantity << std::endl;
        
        uint32_t minQuantity = std::min(bestAsk.quantity, bestBid.quantity);
        
        instrument::Order newAsk = {
            .time = bestAsk.time,
            .price = bestAsk.price,
            .quantity = bestAsk.quantity - minQuantity
        };
        
        instrument::Order newBid = {
            .time = bestBid.time,
            .price = bestBid.price,
            .quantity = bestBid.quantity - minQuantity
        };

        books.asks.clearBest();
        books.bids.clearBest();

        if (newAsk.quantity > 0) {
            books.asks.placeOrder(newAsk);
        }
        if (newBid.quantity > 0) {
            books.bids.placeOrder(newBid);
        }
    }
}

uint32_t Market::getPrice(instrument::ticker ticker) {
    Market::books& books = this->market.at(ticker);
    if (books.asks.isEmpty() || books.bids.isEmpty()) {
        return 0;
    }
    uint32_t askPrice = books.asks.getBest().price;
    uint32_t bidPrice = books.bids.getBest().price;
    //std::cout << "price: " << askPrice << " " << bidPrice << std::endl;
    std::cout << (bidPrice + ((askPrice - bidPrice) / 2)) << std::endl;
    return (bidPrice + ((askPrice - bidPrice) / 2));
}