#include "ChatServer.hpp"
#include "websocketpp/common/system_error.hpp"
#include <mutex>

ChatServer::ChatServer()
{
    chat_server.set_pong_timeout(5000);
    chat_server.init_asio();

    chat_server.set_open_handler(bind(&ChatServer::on_open, this, std::placeholders::_1));
    chat_server.set_close_handler(bind(&ChatServer::on_close, this, std::placeholders::_1));
    chat_server.set_message_handler(bind(&ChatServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

ChatServer::~ChatServer()
{
    chat_server.stop();
    if (server_thread.joinable()) server_thread.join();
}

void ChatServer::on_open(connection_hdl hdl)
{
    std::lock_guard<std::mutex> lock(server_mutex);
    user_list.insert(hdl);
    std::cout << "Подключился новый пользователь. Всего: " << user_list.size() << std::endl;

    websocketpp::lib::error_code ec;
    chat_server.send(hdl, db.get_history_json(10), websocketpp::frame::opcode::text, ec);

    if (ec)
    {
        std::cout << "Ошибка отправки истории сообщения: " << ec.message() << std::endl;
    }
}

void ChatServer::on_close(connection_hdl hdl)
{
    std::lock_guard<std::mutex> lock(server_mutex);
    user_list.erase(hdl);
    std::cout << "Пользователь отключился. Всего: " << user_list.size() << std::endl;
}

void ChatServer::on_message(connection_hdl hdl, server::message_ptr msg)
{
    std::lock_guard<std::mutex> lock(server_mutex);
    // Сохраняем сообщение в базе данных
    db.insert_message(msg->get_payload());
    for (auto it : user_list)
    {
        // Проверка на то, что мы 
        // повторно не отправляем сообщение отправителю
        if (it.owner_before(hdl) || hdl.owner_before(it)) 
        {
            websocketpp::lib::error_code ec;
            chat_server.send(it, msg->get_payload(), msg->get_opcode());

            if (ec)
            {
                std::cout << "Ошибка на стороне клиента: " << ec.message() << std::endl;
            }
        }
    }
}

void ChatServer::run(const uint16_t port)
{
    chat_server.listen(port);
    chat_server.start_accept();
    std::cout << "Сервер успешно запущен на порту: " << port << std::endl;
    server_thread = std::thread([this]() { chat_server.run(); });
}