#ifndef CHAT_CLIENT_HPP
#define CHAT_CLIENT_HPP

#include "websocketpp/common/connection_hdl.hpp"
#include "websocketpp/roles/client_endpoint.hpp"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <thread>

typedef websocketpp::client<websocketpp::config::asio> client;
using websocketpp::connection_hdl;

class ChatClient
{
private:
    client user_client;       // Клиент
    connection_hdl user_hdl;  // Обработчик подключения пользователя
    std::string nickname;     // Никнейм пользователя
    std::thread user_thread;  // Клиентский поток (в дальнейшем он будет удален)
public:
    ChatClient(std::string nickname);
    ~ChatClient();
    void connect(const std::string& uri);
    void send(const std::string& msg);
};

#endif // CHAT_CLIENT_HPP