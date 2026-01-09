#include "ChatErrors.hpp"
#include "config.hpp"
#include "DataManager.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>

DatabaseManager::DatabaseManager()
{
    auto err = sqlite3_open(my_config::DATABASE, &database);
    if (err != SQLITE_OK) 
        throw chat_errors::DatabaseError(chat_errors::DATABASE_OPEN_ERROR, err); 

    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS messages ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "nickname TEXT,"
                               "text TEXT,"
                               "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";

    sqlite3_exec(database, create_table_sql, 0, 0, 0);
}

DatabaseManager::~DatabaseManager()
{
    sqlite3_close(database);
}

void DatabaseManager::insert_message(std::string json_msg)
{
    auto json_msg_parse = nlohmann::json::parse(json_msg);
    // Если json не является по типу сообщением - ничего не делаем 
    if (!json_msg_parse.contains("type") || json_msg_parse["type"] != "message") return;

    auto nickname = json_msg_parse.value("nickname", "unknown");
    auto message = json_msg_parse.value("text", "");

    const char* insert_sql = "INSERT INTO messages (nickname, text) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    auto err = sqlite3_prepare_v2(database, insert_sql, -1, &stmt, 0);
    if (err != SQLITE_OK)
        throw chat_errors::DatabaseError(chat_errors::PREPARE_INSERT_ERROR);

    sqlite3_bind_text(stmt, 1, nickname.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, message.c_str(), -1, SQLITE_TRANSIENT);

    err = sqlite3_step(stmt);
    if (err != SQLITE_DONE) 
        throw chat_errors::DatabaseError(chat_errors::EXECUTE_INSERT_ERROR);

    sqlite3_finalize(stmt);
}

std::string DatabaseManager::get_history_json(int k)
{
    const char* get_sql = "SELECT nickname, text FROM (SELECT * FROM messages ORDER BY id DESC LIMIT ?) ORDER BY id ASC;";
    sqlite3_stmt* stmt;

    auto err = sqlite3_prepare_v2(database, get_sql, -1, &stmt, 0);
    if (err != SQLITE_OK)
        throw chat_errors::DatabaseError(chat_errors::RESTORE_HISTORY_ERROR);

    nlohmann::json history_response;
    history_response["type"] = "history";
    history_response["messages"] = nlohmann::json::array();

    sqlite3_bind_int(stmt, 1, k);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        nlohmann::json msg_obj;
        msg_obj["nickname"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        msg_obj["text"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        
        // Добавляем объект сообщения в массив
        history_response["messages"].push_back(msg_obj);
    }

    sqlite3_finalize(stmt);

    return history_response.dump();
}