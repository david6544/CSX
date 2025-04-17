#include <iostream>
#include "MarketServer.h"

#include "rpc/server.h"
#include "rpc/this_handler.h"

MarketServer::MarketServer(Market& market) : m(market) {}

int MarketServer::runServer() {
    std::cout << "starting server" << std::endl;
    rpc::server srv = rpc::server(rpc::constants::DEFAULT_PORT);

    srv.bind("getPrice", getPrice);
    srv.bind("askOrder", askOrder);
    srv.bind("bidOrder", bidOrder);

    srv.run();

    
    return 0;
}