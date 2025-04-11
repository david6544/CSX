#include "Orderbook.h"
#include <iostream>

int main(void) {

    instrument::Order o1 = {
        o1.time = 21,
        o1.price = 124,
        o1.quantity = 20,
    };

    instrument::Order o2 = {
        o2.time = 22,
        o2.price = 124,
        o2.quantity = 30,
    };

    instrument::Order o3 = {
        o3.time = 23,
        o3.price = 123,
        o3.quantity = 40,
    };

    instrument::Order o4 = {
        o4.time = 24,
        o4.price = 123,
        o4.quantity = 50,
    };



    //place a bunch of bids
    Orderbook bids = Orderbook<instrument::BidOrder>();
    bids.placeOrder(o1);
    bids.placeOrder(o2);
    bids.placeOrder(o3);
    bids.placeOrder(o4);

    std::cout << sizeof(o1) << std::endl;
    std::cout << sizeof(bids) << std::endl;


    std::cout << bids.orders.size() << std::endl;
    while (!bids.orders.empty()) {
        auto o = bids.orders.top();
        std::cout << o.price << " " << o.time << " " << o.quantity << std::endl;
        bids.orders.pop();
    }

    return 0;
}