#ifndef MY_CONFIG
#define MY_CONFIG

#include <string>

namespace my_config
{
    inline constexpr const int PORT = 8080;
    inline std::string HOST = "localhost";
    inline const char* DATABASE = "chat_history.db";
}

#endif // MY_CONFIG