#ifndef CHAT_SERVER_HPP
#define CHAT_SERVER_HPP

#include "DataManager.hpp"
#include <mutex>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>
#include <thread>

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::connection_hdl;

class ChatServer
{
private:
    typedef std::set<connection_hdl, std::owner_less<connection_hdl>> connection_list;

    DatabaseManager db;        // База данных
    connection_list user_list; // Контейнер участников чата
    server chat_server;        // Cервер с чатом
    std::thread server_thread; // Поток, в котором будет запускаться сервер
    std::mutex server_mutex;   // Мьютекс сервера
public:
    ChatServer();
    ~ChatServer();

    void on_open(connection_hdl hdl);  // Добавление нового пользователя в чат
    void on_close(connection_hdl hdl); // Удаление старого пользователя из чата
    void on_message(connection_hdl hdl, server::message_ptr msg); // Отправка сообщения всем участникам чата
    void run(const uint16_t port); // Запуск сервера
};

#endif //CHAT_SERVER_HPP