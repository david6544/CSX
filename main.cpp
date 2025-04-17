#include "rpc/client.h"
#include "MarketServer.h"
#include "Tickers.h"
#include "Utils.h"
#include <iostream>

int main(void) {

    instrument::Order o1 = {
        o1.time = utils::getCurrentTime(),
        o1.price = 50,
        o1.quantity = 20,
    };

    instrument::Order o2 = {
        o2.time = utils::getCurrentTime(),
        o2.price = 100,
        o2.quantity = 30,
    };

    instrument::Order o3 = {
        o3.time = utils::getCurrentTime(),
        o3.price = 150,
        o3.quantity = 40,
    };

    instrument::Order o4 = {
        o4.time = utils::getCurrentTime(),
        o4.price = 200,
        o4.quantity = 50,
    };

    instrument::Order a1 = {
        .time = utils::getCurrentTime(),
        .price = 255,
        .quantity = 20,
    };

    instrument::Order a2 = {
        .time = utils::getCurrentTime(),
        .price = 205,
        .quantity = 30,
    };

    instrument::Order a3 = {
        .time = utils::getCurrentTime(),
        .price = 155,
        .quantity = 40,
    };

    Market m = Market();
    instrument::ticker ticker1 = TickerNames::BOOGLE;
    m.addTicker(ticker1);
    m.placeAsk(ticker1, a1);
    m.placeAsk(ticker1, a2);
    m.placeAsk(ticker1, a3);
    
    m.placeBid(ticker1, o1);
    m.placeBid(ticker1, o2);
    m.placeBid(ticker1, o3);

    std::cout << "price of BOOGLE : " << m.getPrice(ticker1) << std::endl;
    MarketServer ms = MarketServer(m);
    ms.runServer();
    return 0;
}