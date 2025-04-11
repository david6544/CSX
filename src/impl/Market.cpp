#include "Market.h"



bool Market::placeBid(instrument::Ticker ticker, instrument::Order order) {
    Market::books tickerBooks = this->market.at(ticker);
    tickerBooks.bids.placeOrder(order);
    instrument::Order bestAsk = tickerBooks.asks.getBest();
    if (!instrument::isOrderEqual(bestAsk, order)) {
        return 0;
    };
    this->matchOrder(tickerBooks);
    return 1;
}

bool Market::placeAsk(instrument::Ticker ticker, instrument::Order order) {
    Market::books tickerBooks = this->market.at(ticker);
    tickerBooks.asks.placeOrder(order);

    instrument::Order bestBid = tickerBooks.bids.getBest();
    if (!instrument::isOrderEqual(bestBid, order)) {
        return 0;
    };
    this->matchOrder(tickerBooks);
    return 1;
}

constexpr void Market::matchOrder(Market::books books) {
    instrument::Order bestAsk = books.asks.orders.top();
    instrument::Order bestBid = books.bids.orders.top();
    if (bestAsk.price <= bestBid.price) {
        uint32_t minQuantity = std::min(bestAsk.quantity, bestBid.quantity);
        bestAsk.quantity -= minQuantity;
        bestBid.quantity -= minQuantity;

        if (bestAsk.quantity == 0) {
            books.asks.orders.pop();
        } else if (bestBid.quantity == 0) {
            books.bids.orders.pop();
        }
    }
} 