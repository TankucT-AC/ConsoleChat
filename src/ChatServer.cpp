#include "ChatServer.hpp"
#include "ChatErrors.hpp"
#include "config.hpp"
#include "websocketpp/common/connection_hdl.hpp"
#include "websocketpp/common/system_error.hpp"
#include <nlohmann/json.hpp>
#include <mutex>

ChatServer::ChatServer()
{
    chat_server.set_pong_timeout(my_config::TIMEOUT);
    chat_server.init_asio();

    chat_server.set_open_handler([this](connection_hdl hdl) {
        std::lock_guard<std::mutex> lock(server_mutex);
        user_list.insert(hdl);
        std::cout << "Подключился новый пользователь. Всего: " << user_list.size() << std::endl;
        websocketpp::lib::error_code ec;
        chat_server.send(hdl, db.get_history_json(my_config::HISTORY_LIMIT), websocketpp::frame::opcode::text, ec);
        if (ec) 
            throw chat_errors::NetworkError(chat_errors::CONNECT_ERROR);
    });

    chat_server.set_close_handler([this](connection_hdl hdl) {
        std::lock_guard<std::mutex> lock(server_mutex);
        user_list.erase(hdl);
        std::cout << "Пользователь отключился. Всего: " << user_list.size() << std::endl;
    });

    chat_server.set_message_handler([this](connection_hdl hdl, server::message_ptr msg) {
        std::lock_guard<std::mutex> lock(server_mutex);
        db.insert_message(msg->get_payload());
        for (auto it : user_list)
        {
            if (it.owner_before(hdl) || hdl.owner_before(it)) 
            {
                websocketpp::lib::error_code ec;
                chat_server.send(it, msg->get_payload(), msg->get_opcode());

                if (ec) throw chat_errors::NetworkError(chat_errors::SEND_ERROR);
            }
        }
    });
}

ChatServer::~ChatServer()
{
    chat_server.stop();
    if (server_thread.joinable()) server_thread.join();
}

void ChatServer::run(const uint16_t port)
{
    chat_server.listen(port);
    chat_server.start_accept();
    std::cout << "Сервер успешно запущен на порту: " << port << std::endl;
    server_thread = std::thread([this]() {
        try 
        {
            chat_server.run();
        }
        catch (const chat_errors::DatabaseError& e)
        {
            auto err = e.error_msg;
            std::cerr << e.what() << std::endl;
            if (err == chat_errors::EXECUTE_INSERT_ERROR || 
                err == chat_errors::PREPARE_INSERT_ERROR)
                chat_server.stop();
        } catch (const chat_errors::NetworkError& e)
        {
            auto err = e.what();
            std::cerr << err << std::endl;
        } catch (const nlohmann::json::exception& e)
        {
            auto err = e.what();
            std::cerr << err << std::endl;
        } catch (...)
        {
            std::cerr << chat_errors::UNKNOWN_ERROR << std::endl;
            chat_server.stop();
        }
    });
}