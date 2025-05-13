#pragma once

#include "Instrument.h"
#include "MarketUiSession.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <iostream>
#include <thread>
#include <future>
#include <string>

namespace msgpack = clmdep_msgpack;

class MarketUiClient {
private:
    net::io_context ioContext;
    websocket::stream<beast::tcp_stream> websocketStream;
    beast::flat_buffer messageBuffer;
    std::string recievedMessage; // change this?
    std::string host;
    std::string port;

public:
    MarketUiClient(const std::string& host, const std::string& port) 
        : host(host), 
          port(port),
          websocketStream(net::make_strand(ioContext))  // Create stream with a strand from io_context
    {
        // Constructor body
    }

    bool connect(){
        try {
            // Resolve the host
            tcp::resolver resolver(ioContext);
            auto const results = resolver.resolve(host, port);

            //Connect to the server
            beast::get_lowest_layer(websocketStream).connect(results);

            //Websocket handshake
            websocketStream.handshake(host, "/");

            return true;
        } catch (const std::exception& error) {
            std::cerr << "Connect error: " << error.what() << "\n";
            return false;
        }
    }

    void sendOrder(const instrument::Order & message) {
        std::stringstream ss;
        msgpack::pack(ss, message);
        std::string binary = ss.str();

        websocketStream.binary(true);
        websocketStream.write(net::buffer(binary));
    }

    instrument::ExternalOrder receiveOrder() {
        websocketStream.read(messageBuffer);

        auto data = messageBuffer.data();

        std::string binary_data(beast::buffers_to_string(data));

        messageBuffer.consume(messageBuffer.size()); //Clear buffer

        // Deserialize from MessagePack to Order
        msgpack::object_handle objectHandle = msgpack::unpack(binary_data.data(), binary_data.size());
        msgpack::object object = objectHandle.get();
        
        instrument::ExternalOrder order;
        object.convert(order);

        std::cout << "Order Recieved @" << order.price << " | " << order.quantity << std::endl;
        return order;
    }

    void close() {
        websocketStream.close(websocket::close_code::normal);
    }

    void run() {
        ioContext.run();
    }
};