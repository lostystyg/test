
#include "boost/asio/buffer.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/post.hpp"
#include "boost/asio/read.hpp"
#include "boost/asio/write.hpp"
#include "boost/system/detail/error_code.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <memory>

boost::asio::io_context context;

boost::asio::ip::tcp::acceptor acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345));
std::shared_ptr<boost::asio::ip::tcp::socket> gsocket;


void Accept() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);
    acceptor.async_accept(*socket, [socket](const boost::system::error_code& _err){
        if (_err) {
            std::cout << "Accepting error: " << _err.message() << std::endl;
        } else {
            gsocket = socket;
            std::cout << "IP: " << gsocket->remote_endpoint().address().to_string() << std::endl;
            std::cout << "Port: " << gsocket->remote_endpoint().port() << std::endl;
        }

        Accept();
    });
}

int main() {
    Accept();
    
    context.run();
    return 0;
}