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

    void insert_message(std::string json_msg);
    std::string get_history_json(int k);
};

#endif // DATABASE_MANAGER_HPP