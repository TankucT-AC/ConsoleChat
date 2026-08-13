#include "ChatClient.hpp"
#include "ChatErrors.hpp"
#include "websocketpp/common/connection_hdl.hpp"
#include "websocketpp/common/system_error.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

ChatClient::ChatClient(std::string nickname) : nickname(nickname) {
  user_client.clear_access_channels(websocketpp::log::alevel::all);
  user_client.init_asio();
  user_client.set_message_handler(
      [this](connection_hdl, client::message_ptr msg) {
        auto json_msg = nlohmann::json::parse(msg->get_payload());
        if (json_msg["type"] == "message") {
          std::cout << "\r\033[2K" << std::flush;

          std::cout << "[" << json_msg.value("nickname", "unknown")
                    << "]: " << json_msg.value("text", "") << std::endl;

          std::cout << "> " << std::flush;
        }

        if (json_msg["type"] == "history") {
          std::cout << "\r\033[2K" << std::flush;
          for (const auto &it : json_msg["messages"]) {
            std::cout << "[" << it.value("nickname", "unknown")
                      << "]: " << it.value("text", "") << std::endl;
          }

          std::cout << "---------------------------" << std::endl;
          std::cout << "> " << std::flush;
        }
      });

  user_client.set_open_handler([this](connection_hdl hdl) {
    user_hdl = hdl;
    std::cout << "\r\033[2K" << std::flush;
    std::cout << "--- Соединение установлено! ---" << std::endl;
    std::cout << "> " << std::flush;
  });
}

void ChatClient::connect(const std::string &uri) {
  websocketpp::lib::error_code ec;
  auto con = user_client.get_connection(uri, ec);

  if (ec)
    throw chat_errors::NetworkError(chat_errors::CONNECT_ERROR);

  user_client.connect(con);
  user_thread = std::thread([this]() {
    try {
      user_client.run();
    } catch (const chat_errors::NetworkError &e) {
      auto err = e.what();
      std::cerr << err << std::endl;
    } catch (const nlohmann::json::exception &e) {
      auto err = e.what();
      std::cerr << err << std::endl;
    } catch (...) {
      std::cerr << chat_errors::UNKNOWN_ERROR << std::endl;
      user_client.stop();
    }
  });
}

void ChatClient::send(const std::string &msg) {
  websocketpp::lib::error_code ec;

  // Отправляем сообщение в виде json
  nlohmann::json json_msg;
  json_msg["nickname"] = nickname;
  json_msg["type"] = "message";
  json_msg["text"] = msg;

  user_client.send(user_hdl, json_msg.dump(), websocketpp::frame::opcode::text,
                   ec);

  if (ec)
    throw chat_errors::NetworkError(chat_errors::SEND_ERROR);
}

ChatClient::~ChatClient() {
  user_client.stop();
  if (user_thread.joinable())
    user_thread.join();
}