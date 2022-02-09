
#include "boost/asio/io_context.hpp"
#include "boost/asio/read.hpp"
#include "boost/asio/write.hpp"
#include "boost/system/detail/error_code.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <thread>

void Write(boost::asio::ip::udp::socket& socket) {
    std::string toWrite;
    std::cin >> toWrite;
    socket.async_send(boost::asio::buffer(toWrite.data(), toWrite.size()), [&socket](boost::system::error_code err, size_t bytes) {
                std::cout << "Sended!!!: " << bytes << std::endl;
                if (err) {
                    std::cout << "Error: " << err.message() << std::endl;
                    return;
                }
                Write(socket);
            });
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage - ip and port" << std::endl;
        return 1;
    }
    std::string ip = argv[1];
    int port = std::stoll (argv[2]);
    boost::asio::io_context context;
    boost::asio::ip::udp::socket socket(context);
    socket.async_connect(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip), port), [&socket](boost::system::error_code err) {
        Write(socket);
    });
    

    context.run();
    return 0;
}