#include "utils.h"

#include <boost/url/params_view.hpp>
#include <boost/url/parse_query.hpp>
#include <boost/url/url_view.hpp>
#include <iostream>

std::unordered_multimap<std::string, std::string> ParseQuery(const std::string& query) {
    std::unordered_multimap<std::string, std::string> params;

    // Пытаемся распарсить query-строку
    auto result = boost::urls::parse_query(query);
    if (!result.has_value()) {
        std::cerr << "Ошибка парсинга query: " << result.error().message() << "\n";
        return params;
    }

    boost::urls::params_encoded_view view = result.value();
    for (auto param : view) {
        params.emplace(std::string(param.key), std::string(param.value));
    }

    return params;
}

namespace beast = boost::beast;
namespace http = beast::http;

http::response<http::string_body> MakeErrorResponse(http::status status, std::string_view message, unsigned version) {
    http::response<http::string_body> res{status, version};
    res.set(http::field::server, "Calendar.SimpleServer");
    res.set(http::field::content_type, "text/plain; charset=utf-8");
    res.keep_alive(false);
    res.body() = std::string(message);
    res.prepare_payload();
    return res;
}

http::response<http::string_body> MakeResponse(unsigned version, std::string body, bool keep_alive) {
    http::response<http::string_body> res{http::status::ok, version};
    res.set(http::field::server, "Calendar.SimpleServer");
    res.set(http::field::content_type, "text/plain; charset=utf-8");
    res.keep_alive(keep_alive);
    res.body() = std::move(body);
    res.prepare_payload();
    return res;
}

void HandleRequest(boost::asio::ip::tcp::socket& socket, http::request<http::string_body>& request) {
    const auto& target_str = std::string(request.target());
    const auto pos = target_str.find('?');
    const std::string path = target_str.substr(0, pos);
    const std::string query = (pos != std::string::npos) ? target_str.substr(pos + 1) : "";
    const auto params = ParseQuery(query);

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    int year = now->tm_year + 1900;
    int month = now->tm_mon + 1;

    try {
        std::string body;

        if (path == "/calendar/month") {
            if (auto it = params.find("year"); it != params.end()) {
                size_t pos = 0;
                year = std::stoi(it->second, &pos);
                if (pos != it->second.size()) throw std::invalid_argument("year");
            }
            if (auto it = params.find("month"); it != params.end()) {
                size_t pos = 0;
                month = std::stoi(it->second, &pos);
                if (month < 1 || month > 12) throw std::out_of_range("month");
                if (pos != it->second.size()) throw std::invalid_argument("month");
            }
            body = PrintMonth(year, month);

        } else if (path == "/calendar/year") {
            if (auto it = params.find("year"); it != params.end()) {
                size_t pos = 0;
                year = std::stoi(it->second, &pos);
                if (pos != it->second.size()) throw std::invalid_argument("year");
            }
            body = PrintYear(year);

        } else {
            http::write(socket, MakeErrorResponse(http::status::not_found, ERR_NOT_FOUND, request.version()));
            return;
        }

        auto response = MakeResponse(request.version(), std::move(body), request.keep_alive());
        http::write(socket, response);

    } catch (const std::invalid_argument&) {
        http::write(socket, MakeErrorResponse(http::status::bad_request, ERR_INVALID_PARAM, request.version()));
    } catch (const std::out_of_range&) {
        http::write(socket, MakeErrorResponse(http::status::bad_request, ERR_OUT_OF_RANGE, request.version()));
    } catch (const std::exception& e) {
        http::write(socket,
                    MakeErrorResponse(http::status::internal_server_error,
                                      std::string("Внутренняя ошибка сервера: ") + e.what(), request.version()));
    }
}