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

void pricePublisherThread(Market& m, std::shared_ptr<MarketUiServer> server, instrument::ticker tickerSymbol) {
    std::cout << "Starting price publisher for ticker " << tickerSymbol << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while (1) {
        try {
            auto price = m.getPrice(tickerSymbol);
            server->broadcastPrice(price, tickerSymbol);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << price << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }  catch (const std::exception& e) {
            std::cerr << "Error in price publisher: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Retry after delay
        }
    }
}

void testServer(Market& m) {
    // Create the server and keep it alive
    auto globalServer = std::make_shared<MarketUiServer>("127.0.0.1", 8081);
    globalServer->start();

    
    // Print confirmation
    std::cout << "WebSocket server started on 127.0.0.1:8081" << std::endl;
    
    std::thread publisherThread(pricePublisherThread, std::ref(m), globalServer, TickerNames::BOOGLE);
    publisherThread.detach();

    // Keep the thread alive
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(void) {
    Market market = Market();
    instrument::ticker ticker1 = TickerNames::BOOGLE;
    market.addTicker(ticker1);

    MarketServer ms = MarketServer(market);
    
    std::thread marketMakerThread(defaultLiquidity, std::ref(market));
    marketMakerThread.detach();

    std::thread uiServerThread(testServer, std::ref(market));
    uiServerThread.detach(); 

    ms.runServer();
    return 0;
}