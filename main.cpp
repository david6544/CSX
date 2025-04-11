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



    Orderbook ob = Orderbook();
    ob.placeOrder(o1);
    ob.placeOrder(o2);
    ob.placeOrder(o3);
    ob.placeOrder(o4);


    std::cout << sizeof(o1) << std::endl;
    std::cout << sizeof(ob) << std::endl;


    std::cout << ob.orders.size() << std::endl;
    while (!ob.orders.empty()) {
        auto o = ob.orders.top();
        std::cout << o.price << " " << o.time << " " << o.quantity << std::endl;
        ob.orders.pop();
    }

    return 0;
}