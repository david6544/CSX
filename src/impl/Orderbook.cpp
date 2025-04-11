#include "Orderbook.h"


template <typename T>
Orderbook<T>::Orderbook() {
    orders = std::priority_queue<instrument::Order, std::deque<instrument::Order, instrument::BidOrder>();
}

template <typename T>
void Orderbook<T>::placeOrder(instrument::Order order) {
    orders.push(order);
    return;
}

template <typename T>
instrument::Order getBest() {
    return orders.top();
}

template <typename T>
uint64_t Orderbook<T>::getMarketDepth() {
    return orders.size();
}