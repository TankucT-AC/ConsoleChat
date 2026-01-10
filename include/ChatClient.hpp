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
    client user_client;       
    connection_hdl user_hdl;  
    std::string nickname;     
    std::thread user_thread;  
public:
    ChatClient(std::string nickname);
    ~ChatClient();

    /**
    * @brief Устанавливает соединение с сервером.
    * @param uri .
    * @return Ничего не возвращает.
    * @throws chat_errors::NetworkError Если не удалось установить соединение с сервером
    */
    void connect(const std::string& uri);

    /**
    * @brief Отправляет сообщение на сервер.
    * @param msg Сообщение, которое отправляется серверу.
    * @return Ничего не возвращает.
    * @throws chat_errors::NetworkError Если не удалось отправить сообщение серверу
    * @throws nlohmann::json::exception Ошибка создания JSON
    */
    void send(const std::string& msg);
};

#endif // CHAT_CLIENT_HPP