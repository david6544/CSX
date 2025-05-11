#include "rpc/client.h"
#include "MarketServer.h"
#include "MarketMaker.h"
#include "Tickers.h"
#include "Utils.h"
#include "OrderQueue.h"

#include <iostream>
#include <thread>

void defaultLiquidity(Market& m) {
    std::cout << "starting market maker" << std::endl;
    MarketMaker marketMaker = MarketMaker();
    marketMaker.setPrice(1000);
    while (1) {
        marketMaker.addLiquidity(m, TickerNames::BOOGLE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        marketMaker.setPrice(m.getPrice(TickerNames::BOOGLE));
    }
}

void staticPublisher(Market& m) {
    while (1) {
        m.getPrice(TickerNames::BOOGLE);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(void) {
    Market market = Market();
    instrument::ticker ticker1 = TickerNames::BOOGLE;
    market.addTicker(ticker1);

    MarketServer ms = MarketServer(market);
    
    std::thread marketMakerThread(defaultLiquidity, std::ref(market));
    marketMakerThread.detach();

    std::thread publisherThread(staticPublisher, std::ref(market));
    publisherThread.detach();

    ms.runServer();
    return 0;
}