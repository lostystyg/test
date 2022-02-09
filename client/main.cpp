
#include "boost/asio/io_context.hpp"
#include "boost/asio/read.hpp"
#include "boost/asio/write.hpp"
#include "boost/system/detail/error_code.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <iterator>
#include <memory>

void Read(std::shared_ptr<boost::asio::ip::udp::socket> serverSocket) {
    auto buffer = std::make_shared<std::string>();
    buffer->resize(100);
    auto ep = std::make_shared<boost::asio::ip::udp::endpoint>();
    serverSocket->async_receive_from(boost::asio::buffer(*buffer), *ep, [serverSocket, buffer, ep](boost::system::error_code err, size_t bytes) {
                    if (err) {
                        std::cout << err.message() << std::endl;
                        return;
                    }

                    std::cout << "Message: \"" << *buffer << "\" from " << ep->address().to_string() << std::endl;
                    Read(serverSocket);
                });
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage - stun's ip" << std::endl;
        return 1;
    }
    std::string ip = argv[1];
    boost::asio::io_context context;
    // std::shared_ptr<boost::asio::ip::udp::acceptor> acceptor;

    auto clientSocket = std::make_shared<boost::asio::ip::udp::socket>(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));
    // clientSocket->async_connect(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip), 12345), [ip, clientSocket, &context](boost::system::error_code err) {
    //     if (err) {
    //         std::cout << "Client connection failed" << std::endl;
    //         return;
    //     }
    //     // auto serverSocket = std::make_shared<boost::asio::ip::udp::socket>(context);
    //     std::cout << clientSocket->local_endpoint().port() << std::endl;

        clientSocket->async_send_to(boost::asio::buffer("some data"),boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip), 12345), [clientSocket](boost::system::error_code err, size_t bytes) {
            if (err) {
                std::cout << "error sending: " << err.message() << std::endl;
                return;
            }
            std::cout << "send complete: " << bytes <<  std::endl;

// buffer->resize(12);
            Read(clientSocket);
        });
        // acceptor = std::make_shared<boost::asio::ip::udp::acceptor>(context, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), clientSocket.local_endpoint().port()));
        // acceptor->async_accept(*serverSocket, [serverSocket](boost::system::error_code err) {
            
        // });


    context.run();

    return 0;
}