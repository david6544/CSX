#include <iostream>
#include "MarketServer.h"

#include "rpc/server.h"
#include "rpc/this_handler.h"

MarketServer::MarketServer(Market& market) : m(market) {}

int MarketServer::runServer() {
    std::cout << "starting server" << std::endl;
    rpc::server srv = rpc::server(rpc::constants::DEFAULT_PORT);

    // It's possible to bind non-capturing lambdas
    srv.bind("getPrice", [this](instrument::ticker ticker) {
        return m.getPrice(ticker); 
    });

    srv.run();

    
    return 0;
}