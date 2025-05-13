#pragma once

#include "MarketUiSession.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <thread>
#include <future>
#include <string>


class MarketUiServer {
private:
    net::io_context ioContext;
    std::shared_ptr<WsListener> listener;
    std::thread serverThread;
    bool running = false;

public:
    MarketUiServer(const std::string& address, unsigned short port) {
        auto const addr = net::ip::make_address(address);
        listener = std::make_shared<WsListener>(ioContext, tcp::endpoint{addr, port});
    }

    ~MarketUiServer() {
        stop();
    }

    void start() {
        if (!running) {
            listener->run();
            running = true;
            serverThread = std::thread([this]() {
                ioContext.run();
            });
        }
    }


    void stop() {
        if (running) {
            ioContext.stop();
            if (serverThread.joinable()) {
                serverThread.join();
            }
            running = false;
        }
    }
};