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

    DatabaseManager db;        
    connection_list user_list; 
    server chat_server;        
    std::thread server_thread; 
    std::mutex server_mutex;   
public:
    ChatServer();
    ~ChatServer();

    /**
    * @brief Запускает сервер.
    * @param port Порт, на котором будет запущен сервер.
    * @return Ничего не возвращает.
    * @throws std::system_error Если порт занят или возникла системная ошибка ASIO.
    */
    void run(const uint16_t port);
};

#endif //CHAT_SERVER_HPP