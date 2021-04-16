//
// Created by niickson on 4/16/21.
//

#include "downloader.hpp"
std::string downloader::download_page(std::string_view url) {
  std::string protocol{};
  std::string host{};
  std::string target{};
  parseUri(url, protocol, host, target);

  if (protocol == "http")
    return download_http_page(std::move(host), std::move(target));
  else if (protocol == "https")
    return download_https_page(std::move(host), std::move(target));
  return "No protocol";
}

std::string downloader::download_https_page(std::string&& host,
                                            std::string&& target) {
  boost::asio::io_context ioc{};

  boost::asio::ssl::context ctx(boost::asio::ssl::context::tls_client);
  ctx.set_default_verify_paths();
  ctx.add_verify_path("/etc/ssl/certs/");
  ctx.set_verify_mode(boost::asio::ssl::verify_peer);

  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::beast::ssl_stream<boost::beast::tcp_stream> stream(ioc, ctx);

  if (!SSL_set_tlsext_host_name(stream.native_handle(), host.data())) {
    boost::beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                boost::asio::error::get_ssl_category()};
    throw boost::beast::system_error{ec};
  }

  auto const results = resolver.resolve(host, "443");

  boost::beast::get_lowest_layer(stream).connect(results);

  stream.handshake(boost::asio::ssl::stream_base::client);

  boost::beast::http::request<boost::beast::http::string_body> req{
      boost::beast::http::verb::get, target, 11};
  req.set(boost::beast::http::field::host, host);
  req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(stream, req);

  boost::beast::flat_buffer buffer;
  boost::beast::http::response<boost::beast::http::string_body> res;

  boost::beast::http::read(stream, buffer, res);

  boost::beast::error_code ec;
  stream.shutdown(ec);

  return res.body();
}

std::string downloader::download_http_page(std::string&& host,
                                           std::string&& target) {
  boost::asio::io_context ioc{};

  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::beast::tcp_stream stream(ioc);

  auto const results = resolver.resolve(host, "80");

  stream.connect(results);
  stream.expires_after(std::chrono::seconds(3));

  boost::beast::http::request<boost::beast::http::string_body> req{
      boost::beast::http::verb::get, target, 11};
  req.set(boost::beast::http::field::host, host);
  req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  boost::beast::http::write(stream, req);

  boost::beast::flat_buffer buffer;
  boost::beast::http::response<boost::beast::http::string_body> res;

  boost::beast::http::read(stream, buffer, res);

  boost::beast::error_code ec;
  stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

  return res.body();
}
void downloader::parseUri(std::string_view url, std::string& protocol,
                          std::string& host, std::string& target) {
  size_t dd_pos = url.find_first_of(':');
  protocol = url.substr(0, dd_pos);
  size_t path_start = url.find('/', dd_pos + 3);
  if (path_start == std::string::npos) {
    host = url.substr(dd_pos + 3, url.size() - dd_pos - 2);
    target = "/";
  } else {
    host = url.substr(dd_pos + 3, path_start - dd_pos - 3);
    target = url.substr(path_start, url.size() - path_start + 1);
  }
}
