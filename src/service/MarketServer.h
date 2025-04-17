#pragma once
#include "Market.h"

class MarketServer {
    public:

    MarketServer(Market& m);
    
    Market& m;
    // Declare the function
    int runServer();
};