
// #include "boost/asio/buffer.hpp"
// #include "boost/asio/io_context.hpp"
// #include "boost/asio/post.hpp"
// #include "boost/asio/read.hpp"
// #include "boost/asio/write.hpp"
// #include "boost/system/detail/error_code.hpp"
// #include <iostream>
// #include <boost/asio.hpp>
// #include <memory>

// boost::asio::io_context context;

// // boost::asio::ip::udp::acceptor acceptor(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 12345));
// boost::asio::ip::udp::socket gsocket (context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 12345));
// auto ep = std::make_shared<boost::asio::ip::udp::endpoint>();


// void Accept() {
//     auto buffer = std::make_shared<std::string>();

//     gsocket.async_receive_from(boost::asio::buffer(*buffer), *ep, [buffer](const boost::system::error_code err, size_t bytes) {
//         if (err) {
//             std::cout << "Error: " << err.message() << std::endl;
//         }
//         if (bytes != 0) {
//         std::cout << "IP: " << ep->address().to_string() << ":"<< ep->port() << std::endl;            
//         }
//         Accept();
//     });
// }

// int main() {
//     Accept();
    
//     context.run();
//     return 0;
// }

#include <string>
#include <iostream>
#include <boost/asio.hpp>
// #include <boost/bind.hpp>

using boost::asio::ip::udp;

namespace {

class HelloWorldServer {
public:
    HelloWorldServer(boost::asio::io_service& io_service)
        : _socket(io_service, udp::endpoint(udp::v4(), 12345))
    {
        startReceive();
    }
private:
    void startReceive() {
        _socket.async_receive_from(
            boost::asio::buffer(_recvBuffer), _remoteEndpoint,
            [this](const boost::system::error_code& error,
                       std::size_t bytes_transferred){this->handleReceive(error, bytes_transferred);});
    }

    void handleReceive(const boost::system::error_code& error,
                       std::size_t bytes_transferred) {
        if (!error || error == boost::asio::error::message_size) {

            auto message = std::make_shared<std::string>("Hello, World\n");
            std::cout << _remoteEndpoint.address().to_string() << ":" << _remoteEndpoint.port() << std::endl;
            startReceive();
        }
    }

    void handleSend(std::shared_ptr<std::string> message,
                    const boost::system::error_code& ec,
                    std::size_t bytes_transferred) {
        startReceive();
    }

    udp::socket _socket;
    udp::endpoint _remoteEndpoint;
    std::array<char, 1024> _recvBuffer;
};

}  // namespace

int main() {
    try {
        boost::asio::io_service io_service;
        HelloWorldServer server{io_service};
        io_service.run();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}