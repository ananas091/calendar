#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <string>
#include <unordered_map>

#include "calendar_print.h"
#include "constants.h"

std::unordered_multimap<std::string, std::string> ParseQuery(const std::string& query);

boost::beast::http::response<boost::beast::http::string_body> MakeErrorResponse(boost::beast::http::status status,
                                                                                std::string_view message,
                                                                                unsigned version);

boost::beast::http::response<boost::beast::http::string_body> MakeResponse(unsigned version, std::string body,
                                                                           bool keep_alive);

void HandleRequest(boost::asio::ip::tcp::socket& socket,
                   boost::beast::http::request<boost::beast::http::string_body>& request);