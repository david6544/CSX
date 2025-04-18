#include <iostream>
#include "Tickers.h"
#include "Instrument.h"

#include "rpc/client.h"
#include "rpc/rpc_error.h"

int main(void) {
    rpc::client c("localhost", rpc::constants::DEFAULT_PORT);


    instrument::ExternalOrder o1 = {
        .price = 50,
        .quantity = 20,
    };

    instrument::ExternalOrder o2 = {
        .price = 100,
        .quantity = 30,
    };

    instrument::ExternalOrder o3 = {
        .price = 1500,
        .quantity = 50000,
    };

    instrument::ExternalOrder o4 = {
        .price = 200,
        .quantity = 50,
    };

    instrument::ExternalOrder a1 = {
        .price = 255,
        .quantity = 20,
    };
    
    instrument::ExternalOrder a2 = {
        .price = 205,
        .quantity = 30,
    };

    instrument::ExternalOrder a3 = {
        .price = 165,
        .quantity = 40,
    };
    
    instrument::ticker ticker1 = TickerNames::BOOGLE;

    try {
        //c.call("askOrder", ticker1, a1);
        //c.call("askOrder", ticker1, a2);
        //c.call("askOrder", ticker1, a3);

        //c.call("bidOrder", ticker1, o1);
        //c.call("bidOrder", ticker1, o2);
        c.call("bidOrder", ticker1, o3);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        
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