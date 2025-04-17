#include <iostream>

#include "rpc/client.h"
#include "rpc/rpc_error.h"

int main(void) {
    rpc::client c("localhost", rpc::constants::DEFAULT_PORT);
    try {
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