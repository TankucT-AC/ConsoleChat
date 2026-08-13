#ifndef MY_CONFIG
#define MY_CONFIG

#include <string>

namespace my_config {
inline constexpr const int PORT = 8080;
inline constexpr const int TIMEOUT = 5000;
inline constexpr const int HISTORY_LIMIT = 10;
inline const std::string HOST = "localhost";
inline const char *DATABASE = "chat_history.db";
} // namespace my_config

#endif // MY_CONFIG