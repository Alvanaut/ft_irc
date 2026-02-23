#include <string>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>

bool valide_Port(char* argv) {
    if (std::strlen(argv) > 5 || std::strlen(argv) < 4)
    {
        std::cout << "Error: port: 1024 - 65535" << std::endl;
        return false;
    }
    for (int i = 0; argv[i]; i++)
    {
        if (!std::isdigit(argv[i]))
        {
            std::cout << "Error: only number in port" << std::endl;
            return false;
        }
    }
    return true;
}

int main(int ac, char** argv)
{
    if (ac != 3)
    {
        std::cout << "Usage: ./ircserver <port> <password>" << std::endl;
        return 1;
    }
    if (!valide_Port(argv[1]))
        return 1;
    int port = std::atoi(argv[1]);
    if (port < 1024 || port > 65535)
    {
        std::cout << "Error: port: 1024 - 65535" << std::endl;
        return 1;
    }
    std::cout << "Server starting on port: " << port << std::endl;
    return 0;
}