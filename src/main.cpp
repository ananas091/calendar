#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <unordered_map>

#include "calendar_print.h"
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
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> request;
            http::read(socket, buffer, request);

            std::string target = std::string(request.target());
            std::string body;

            auto pos = target.find('?');
            std::string path = target.substr(0, pos);
            std::string query = (pos != std::string::npos) ? target.substr(pos + 1) : "";
            auto params = ParseQuery(query);

            int year = 1970, month = 1;

            if (path == "/calendar/month") {
                if (params.count("year")) {
                    year = std::stoi(params["year"]);
                }
                if (params.count("month")) {
                    month = std::stoi(params["month"]);
                }
                body = PrintMonth(year, month);
            } else if (path == "/calendar/year") {
                if (params.count("year")) {
                    year = std::stoi(params["year"]);
                }
                body = PrintYear(year);
            } else {
                body = "Неизвестный путь. Используйте /calendar/month или /calendar/year.\n";
            }

            http::response<http::string_body> response{http::status::ok, request.version()};
            response.set(http::field::server, "Calendar.SimpleServer");
            response.set(http::field::content_type, "text/plain; charset=utf-8");
            response.keep_alive(false);
            response.body() = body;
            response.prepare_payload();

            http::write(socket, response);

            beast::error_code ec;
            socket.shutdown(tcp::socket::shutdown_send, ec);
        }

    } catch (std::exception const& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
