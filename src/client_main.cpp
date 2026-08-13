#include "ChatClient.hpp"
#include "config.hpp"

int main() {
  std::cout << "Введите свой никнейм: ";
  std::string nickname;
  std::cin >> nickname;

  // Очищаем буфер от остатков '\n' и прочего мусора
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  ChatClient c(nickname);
  c.connect("ws://" + my_config::HOST + ":" + std::to_string(my_config::PORT));

  std::string input;
  while (true) {
    std::cout << "> ";
    std::getline(std::cin, input);
    if (input == "exit")
      break;
    c.send(input);
  }

  return 0;
}