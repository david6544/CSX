#include <iostream>
#include "Instrument.h"
#include "Utils.h"
#include "Tickers.h"

#include "rpc/client.h"
#include "rpc/rpc_error.h"

int main(void) {
    rpc::client c("localhost", rpc::constants::DEFAULT_PORT);


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
        .price = 165,
        .quantity = 40,
    };
    
    instrument::ticker ticker1 = TickerNames::BOOGLE;

    try {
        c.call("askOrder", ticker1, a1);
        c.call("askOrder", ticker1, a2);
        c.call("askOrder", ticker1, a3);

        c.call("bidOrder", ticker1, o1);
        c.call("bidOrder", ticker1, o2);
        c.call("bidOrder", ticker1, o3);

        
        std::cout << "getPrice(BOOGLE) = ";
        uint32_t price = c.call("getPrice", 0).as<uint32_t>();
        std::cout << price  << std::endl;
    } catch (rpc::rpc_error &e) {
        std::cout << std::endl << e.what() << std::endl;
        std::cout << "in function '" << e.get_function_name() << "': ";

        using err_t = std::tuple<int, std::string>;
        auto err = e.get_error().as<err_t>();
        std::cout << "[error " << std::get<0>(err) << "]: " << std::get<1>(err)
                  << std::endl;
        return 1;
    }

    return 0;
}