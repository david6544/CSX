#pragma once

#include "Instrument.h"
#include "UiEndpoints.h"


#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector> 




namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace msgpack = clmdep_msgpack;

void fail(beast::error_code errorCode, char const* error) {
    std::cerr << error << ": " << errorCode.message() << "\n";
}


class MarketUiSession : public std::enable_shared_from_this<MarketUiSession> {
    //possibly look into udp for *blazingly fast* request
    websocket::stream<beast::tcp_stream> webSocketStream;
    beast::flat_buffer messageBuffer;   
    bool active = true;

    public:
    explicit MarketUiSession(tcp::socket&& socket)
        : webSocketStream(std::move(socket))
    {}

    bool isActive() const {
        return active;
    }

    void runServer() {
        //Set suggested timeout settings for websocket
        webSocketStream.set_option(
            websocket::stream_base::timeout::suggested(
                beast::role_type::server
            ));
        
        //Set a decorator to change the Server of the handshake
        webSocketStream.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& response) {
                response.set(http::field::server,
                std::string(BOOST_BEAST_VERSION_STRING) + " market-ui-server");
            }
        ));

        //Accept websocket handshake
        webSocketStream.async_accept(
            beast::bind_front_handler(
                &MarketUiSession::onAccept,
                shared_from_this()
        ));
    }

    void onAccept(beast::error_code errorCode) {
        if (errorCode) {
            active = false;
            return fail(errorCode, "accept");
        }

        std::cout << "Websocket client connected" << std::endl;

        //Read message
        active = true;
        doRead();
    }

    void doRead() {
        webSocketStream.async_read(
            messageBuffer,
            beast::bind_front_handler(
                &MarketUiSession::onRead,
                shared_from_this()
        ));
    }

    // New method to extract message type and payload
    std::pair<uint8_t, std::string> extractMessageHeader(const std::string& binaryData) {
        if (binaryData.size() < 4) {
            throw std::runtime_error("Message too short to contain header");
        }
        
        // Extract message type (first byte)
        uint8_t messageType = static_cast<uint8_t>(binaryData[0]);
        
        // Extract payload length (next 3 bytes, big-endian)
        uint32_t payloadLength = 
            ((static_cast<uint32_t>(static_cast<uint8_t>(binaryData[1])) << 16) |
             (static_cast<uint32_t>(static_cast<uint8_t>(binaryData[2])) << 8) |
             static_cast<uint32_t>(static_cast<uint8_t>(binaryData[3])));
        
        // Validate payload length
        if (binaryData.size() < 4 + payloadLength) {
            throw std::runtime_error("Message truncated");
        }
        
        // Extract payload
        std::string payload = binaryData.substr(4, payloadLength);
        
        return std::make_pair(messageType, payload);
    }

    void onRead(beast::error_code errorCode, std::size_t transferBytes) {
        boost::ignore_unused(transferBytes);

        if (errorCode == websocket::error::closed) {
            std::cout << "Websocket connection closed" << std::endl;
            return;
        }

        if (errorCode) {
            fail(errorCode, "read");
        }

        auto data = messageBuffer.data();
        std::string msgData(beast::buffers_to_string(data));

        std::cout << "Received " << transferBytes << " bytes" 
                  << (webSocketStream.got_binary() ? " (binary)" : " (text)") << std::endl;

        if (!webSocketStream.got_binary()) {
            // Handle text message - could be JSON or just plain text
            std::cout << "Text message: " << msgData << std::endl;
            handleTextMessage(msgData);
        } else {
            // Handle binary message with header
            try {
                // Extract message type and payload
            
                endpoints::MessageType type = static_cast<endpoints::MessageType>(msgData[0]);
                std::string payload = msgData.substr(1);

                std::cout << msgData << std::endl;

                // Process based on message type
                switch (type) {
                    case endpoints::MessageType::OrderSubmit:
                        handleOrderMessage(payload);
                        break;
                    case endpoints::MessageType::PriceRequest:
                        handlePriceRequestMessage(payload);
                        break;
                    case endpoints::MessageType::MarketData:
                        handleMarketDataMessage(payload);
                        break;
                    default:
                        throw std::runtime_error("Unknown message type");
                }
            } catch (const std::exception& e) {
                webSocketStream.text(true);
                std::string errorMsg = "Error: " + std::string(e.what());
                std::cout << errorMsg << std::endl;
                webSocketStream.async_write(
                    net::buffer(errorMsg),
                    beast::bind_front_handler(&MarketUiSession::onWrite, shared_from_this())
                );
                return;
            }
        }
        
        // Clear the buffer for the next message
        messageBuffer.consume(messageBuffer.size());
        
        // Continue reading
        doRead();
    }

    // New handler methods for each message type
    void handleOrderMessage(const std::string& payload) {
        // Deserialize order from MessagePack payload
        msgpack::object_handle handle = msgpack::unpack(payload.data(), payload.size());
        msgpack::object object = handle.get();

        std::cout << payload.data() << " " << payload.size() << std::endl;
        
        // Extract order details
        instrument::ExternalOrder order;
        object.convert(order);
        
        std::cout << "Received Order: " << order.price << " | " << order.quantity << std::endl;
        processOrder(order);
    }

    
    void handlePriceRequestMessage(const std::string& payload) {
        // Deserialize price request from MessagePack payload
        msgpack::object_handle handle = msgpack::unpack(payload.data(), payload.size());
        msgpack::object object = handle.get();
        
        // Extract symbol
        std::string symbol;
        
        
        std::cout << "Price request for symbol: " << symbol << std::endl;
        
        // Here you would handle the price request
        // For example: sendPriceResponse(symbol);
    }
    
    void handleMarketDataMessage(const std::string& payload) {
        // Similar implementation as above
        std::cout << "Market data request received" << std::endl;
    }

    // Example order processing
    void processOrder(const instrument::ExternalOrder& order) {
        // Here you would:
        // 1. Add the order to your market system
        // 2. Match it against existing orders
        // 3. Execute trades if there's a match
        
        std::cout << "Processing order - Price: " << order.price 
                  << ", Quantity: " << order.quantity << std::endl;
        
        // This is where you'd integrate with your Market class
    }


    // Process text messages (could implement a simple protocol)
    void handleTextMessage(const std::string& text) {
        // Echo back text messages with a prefix
        std::string response = "Server received: " + text;
        
        webSocketStream.text(true);
        webSocketStream.async_write(
            net::buffer(response),
            beast::bind_front_handler(&MarketUiSession::onWrite, shared_from_this())
        );
    }

    

    void publishTickerPrice(const uint32_t& price, const uint16_t& ticker) {
        try {
            std::string message;
            message.push_back(static_cast<char>(endpoints::MessageType::MarketData));

            endpoints::PriceUpdate update{ticker, price};
            
            std::stringstream buffer;
            msgpack::pack(buffer, update);
            message += buffer.str();
            
            // Set binary mode for MessagePack data
            webSocketStream.binary(true);
            webSocketStream.async_write(
                net::buffer(message),
                beast::bind_front_handler(&MarketUiSession::onWrite, shared_from_this())
            );

        } catch (const std::exception& e) {
            std::cerr << "Error publishing price: " << e.what() << "\n";
            active = false;
        }
    }

    void onWrite(beast::error_code errorCode, std::size_t transferBytes) {
        boost::ignore_unused(transferBytes);

        if (errorCode) {
            // Handle the "operation_canceled" error gracefully
            if (errorCode == net::error::operation_aborted) {
                std::cerr << "Write operation canceled (client disconnected)\n";
                active = false;  // Mark session as inactive
                return;
            }
            
            // Handle other errors
            fail(errorCode, "write");
            active = false;
            return;
        }
    }
};

class WsListener : public std::enable_shared_from_this<WsListener> {
    
    net::io_context& ioContext;
    tcp::acceptor acceptor;

    //track all active sessions
    std::vector<std::shared_ptr<MarketUiSession>> activeSessions;
    std::mutex sessionsMutex;

    public:
    
    WsListener(
        net::io_context& ioContext,
        tcp::endpoint endpoint 
    ) : ioContext(ioContext), acceptor(ioContext) {
        beast::error_code errorCode;

        //Open acceptor
        acceptor.open(endpoint.protocol(), errorCode);

        if (errorCode) {
            fail(errorCode, "open");
            return;
        }

        //Allow address reuse
        acceptor.set_option(net::socket_base::reuse_address(true), errorCode);

        if (errorCode) {
            fail(errorCode, "set_option");
            return;
        }

        //Bind endpoint to server address
        acceptor.bind(endpoint, errorCode);

        if (errorCode) {
            fail(errorCode, "bind");
            return;
        }

        //Start listening for connections
        acceptor.listen(net::socket_base::max_listen_connections, errorCode);

        if (errorCode) {
            fail(errorCode, "listen");
            return;
        }
    }


    void run() {
        doAccept();
    }
    
    void doAccept() {
        // New connection get it's own strand (essentially its own thread)
        acceptor.async_accept(
            net::make_strand(ioContext),
            beast::bind_front_handler(
                &WsListener::onAccept,
                shared_from_this()
        ));
    }

    void onAccept(beast::error_code errorCode, tcp::socket socket) {
            if (errorCode) {
            fail(errorCode, "listen");
        } else {
            //Create session and run
            auto session = std::make_shared<MarketUiSession>(std::move(socket));
            
            std::lock_guard<std::mutex> lock(sessionsMutex);
            activeSessions.push_back(session);

            session->runServer();
        }

        doAccept();
    }

    void broadcastPrice(const uint32_t& price, const uint16_t& ticker) {
        std::lock_guard<std::mutex> lock(sessionsMutex);

        activeSessions.erase(
            std::remove_if(activeSessions.begin(), activeSessions.end(),
            [](const std::shared_ptr<MarketUiSession>& session) {
                return !session->isActive();
            }),
            activeSessions.end());

        for (auto& session : activeSessions) {
            session->publishTickerPrice(price, ticker);
        }
    }
};