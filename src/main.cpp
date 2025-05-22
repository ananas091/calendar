#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <ctime>
#include <iostream>
#include <unordered_map>

#include "calendar_print.h"
#include "constants.h"
#include "utils.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int main() {
    try {
        asio::io_context ioc;
        tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};
        std::cout << "Сервер слушает порт 8080...\n";

        while (true) {
            try {
                tcp::socket socket{ioc};
                boost::system::error_code accept_ec;

                acceptor.accept(socket, accept_ec);
                if (accept_ec) {
                    std::cerr << "Ошибка accept: " << accept_ec.message() << "\n";
                    continue;
                }

                beast::flat_buffer buffer;
                http::request<http::string_body> request;
                beast::error_code read_ec;

                http::read(socket, buffer, request, read_ec);
                if (read_ec) {
                    beast::error_code ignored_ec;
                    http::write(socket,
                                MakeErrorResponse(http::status::internal_server_error, "Ошибка чтения запроса", 11),
                                ignored_ec);
                    continue;
                }

                HandleRequest(socket, request);

                beast::error_code shutdown_ec;
                socket.shutdown(tcp::socket::shutdown_send, shutdown_ec);
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при обработке соединения: " << e.what() << "\n";
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Фатальная ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
