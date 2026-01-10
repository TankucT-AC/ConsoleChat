#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <sqlite3.h>
#include <string>

class DatabaseManager
{
private:
    sqlite3 *database;
public:
    DatabaseManager();
    ~DatabaseManager();

    /**
    * @brief Записывает сообщение в базу данных.
    * @param json_msg Данные о сообщении в JSON.
    * @return Ничего не возвращает.
    * @throws chat_errors::DatabaseError если запрос к БД не удался.
    */
    void insert_message(std::string json_msg);

    /**
    * @brief Загружает последние k сообщений из базы.
    * @param k Количество сообщений.
    * @return JSON-строка с массивом объектов сообщений.
    * @throws chat_errors::DatabaseError если запрос к БД не удался.
    */
    std::string get_history_json(int k);
};

#endif // DATABASE_MANAGER_HPP