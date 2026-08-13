#ifndef CHAT_ERRORS_HPP
#define CHAT_ERRORS_HPP

#include <stdexcept>
#include <string>

namespace chat_errors {
/*ЛОГИ ОШИБОК:*/

// Ошибки базы данных
inline const std::string DATABASE_OPEN_ERROR = "Не удалось открыть файл базы";
inline const std::string PREPARE_INSERT_ERROR =
    "Не удалось подготовить оператор INSERT";
inline const std::string EXECUTE_INSERT_ERROR =
    "Не удалось выполнить операцию INSERT";
inline const std::string RESTORE_HISTORY_ERROR =
    "Не удалось восстановить историю чата";

// Ошибки cети
inline const std::string CONNECT_ERROR = "Не удалось установить соединение";
inline const std::string SEND_ERROR = "Не удалось отправить сообщение";
inline const std::string UNKNOWN_ERROR = "Неизвестная ошибка";

/*ИСКЛЮЧЕНИЯ:*/

// Базовое исключение для нашего проекта
class ChatException : public std::runtime_error {
public:
  explicit ChatException(const std::string &msg) : std::runtime_error(msg) {}
};

// Ошибки базы данных
class DatabaseError : public ChatException {
public:
  std::string error_msg;
  explicit DatabaseError(const std::string &msg, int code = -1)
      : ChatException("DB Error (" + std::to_string(code) + "): " + msg),
        error_msg(msg) {}
};

// Ошибки парсинга протокола (JSON)
class ProtocolError : public ChatException {
public:
  explicit ProtocolError(const std::string &msg)
      : ChatException("Protocol Error: " + msg) {}
};

// Ошибки сети (WebSocket)
class NetworkError : public ChatException {
public:
  explicit NetworkError(const std::string &msg)
      : ChatException("Network Error: " + msg) {}
};

} // namespace chat_errors

#endif // CHAT_ERRORS_HPP