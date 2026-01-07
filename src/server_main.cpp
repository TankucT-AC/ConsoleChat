#include "ChatServer.hpp"
#include "config.hpp"

int main()
{
    ChatServer s;
    s.run(my_config::PORT);

    std::string command;
    while (true)
    {
        std::getline(std::cin, command);
        if (command == "exit") break;
    }
    return 0;
}