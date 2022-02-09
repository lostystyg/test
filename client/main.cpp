
#include "boost/asio/io_context.hpp"
#include "boost/asio/read.hpp"
#include "boost/asio/write.hpp"
#include "boost/system/detail/error_code.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <iterator>
#include <memory>

void Read(std::shared_ptr<boost::asio::ip::tcp::socket> serverSocket) {
    auto buffer = std::make_shared<std::string>();
    buffer->resize(100);
    serverSocket->async_receive(boost::asio::buffer(*buffer), [serverSocket, buffer](boost::system::error_code err, size_t bytes) {
                    if (err) {
                        std::cout << err.message() << std::endl;
                        return;
                    }

                    std::cout << "Message: " << *buffer << std::endl;
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
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;

    boost::asio::ip::tcp::socket clientSocket(context);
    clientSocket.async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), 12345), [&clientSocket, &context, &acceptor](boost::system::error_code err) {
        if (err) {
            std::cout << "Client connection failed" << std::endl;
            return;
        }
        auto serverSocket = std::make_shared<boost::asio::ip::tcp::socket>(context);
        acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), clientSocket.local_endpoint().port()));
        acceptor->async_accept(*serverSocket, [serverSocket](boost::system::error_code err) {
            if (err) {
                std::cout << "Error accepting: " << err.message() << std::endl;
                return; 
            }
            // buffer->resize(12);
            Read(serverSocket);
        });
    });

    context.run();

    return 0;
}