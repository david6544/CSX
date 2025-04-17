#include "rpc/client.h"
#include "MarketServer.h"
#include "Tickers.h"
#include "Utils.h"
#include <iostream>

int main(void) {
    Market m = Market();
    
    instrument::ticker ticker1 = TickerNames::BOOGLE;
    m.addTicker(ticker1);

    MarketServer ms = MarketServer(m);
    ms.runServer();
    return 0;
}