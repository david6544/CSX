#include "rpc/client.h"
#include "MarketServer.h"
#include "MarketMaker.h"
#include "Tickers.h"
#include "Utils.h"
#include "OrderQueue.h"
#include "MarketUiClient.h"
#include "MarketUiServer.h"

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

void testServer() {
    // Create the server and keep it alive
    auto globalServer = new MarketUiServer("127.0.0.1", 8081);
    globalServer->start();

    
    // Print confirmation
    std::cout << "WebSocket server started on 127.0.0.1:8081" << std::endl;
    
    // Keep the thread alive
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "WebSocket server still running..." << std::endl;
    }
}

int main(void) {
    Market market = Market();
    instrument::ticker ticker1 = TickerNames::BOOGLE;
    market.addTicker(ticker1);

    MarketServer ms = MarketServer(market);
    
    //std::thread marketMakerThread(defaultLiquidity, std::ref(market));
    //marketMakerThread.detach();

    //std::thread publisherThread(staticPublisher, std::ref(market));
    //publisherThread.detach();

    std::thread uiServerThread(testServer);
    uiServerThread.detach();

    ms.runServer();
    return 0;
}