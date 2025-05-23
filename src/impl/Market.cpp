#include "Market.h"

Market::Market() {
    //do the uniqe ptr stuff here
    this->market = std::array<Market::books, marketSize>();

    for (auto& price : cachedPrices) {
        price.store(0, std::memory_order_relaxed);
    }

    this->executionThread = std::thread(&Market::processOrdersLoop, this);
    this->executionThread.detach();
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

void Market::placeBid(instrument::ticker ticker, const instrument::Order & order) {
    Market::books& tickerBooks = this->market.at(ticker);
    tickerBooks.bids.placeOrder(order);
}

void Market::placeAsk(instrument::ticker ticker, const instrument::Order& order) {
    Market::books& tickerBooks = this->market.at(ticker);
    tickerBooks.asks.placeOrder(order);
}

void Market::processOrdersLoop() {
    while (true) {
        //Process all tickers
        for (instrument::ticker ticker = 0; ticker < this->marketSize; ++ticker) {
            if (ticker >= market.size()) continue;

            auto& tickerBooks = market[ticker];

            tickerBooks.bids.executeOrder();
            tickerBooks.asks.executeOrder();

            if (!tickerBooks.bids.isEmpty() && !tickerBooks.asks.isEmpty()) {
                matchOrder(tickerBooks, ticker);
            }
        }

        //Throttle for cpu
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

void Market::matchOrder(Market::books& books, instrument::ticker ticker) {

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

    //update cachedPrice
    if (!books.asks.isEmpty() && !books.bids.isEmpty()) {
        uint32_t askPrice = books.asks.getBest().price;
        uint32_t bidPrice = books.bids.getBest().price;
        uint32_t midPrice = bidPrice + ((askPrice - bidPrice) / 2);
        
        // Store price atomically
        cachedPrices[ticker].store(midPrice, std::memory_order_release);
    }
}

uint32_t Market::getPrice(instrument::ticker ticker) {

    uint32_t cachedPrice = cachedPrices[ticker].load(std::memory_order_acquire);

    if (cachedPrice > 0) {
        return cachedPrice;
    }
    
    Market::books& books = this->market.at(ticker);
    if (books.asks.isEmpty() || books.bids.isEmpty()) {
        return 0;
    }

    uint32_t askPrice = books.asks.getBest().price;
    uint32_t bidPrice = books.bids.getBest().price;
    uint32_t midPrice = bidPrice + ((askPrice - bidPrice) / 2);
    
    cachedPrices[ticker].store(midPrice, std::memory_order_release);
    
    return midPrice;
}