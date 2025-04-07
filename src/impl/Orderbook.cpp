#include "Orderbook.h"

Orderbook::Orderbook() {
    orders = std::priority_queue<instrument::Order, std::deque<instrument::Order>, instrument::OrderMatch>();
}

void Orderbook::placeOrder(instrument::Order order) {
    orders.push(order);
    return;
}