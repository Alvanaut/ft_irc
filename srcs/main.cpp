#include "../includes/Command.hpp"
#include "../includes/Server.hpp"
#include <iostream>

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Please specify only a port and a password" << std::endl;
		return 1;
	}
	
	Server	server(av[1], av[2]);
	try
	{
		server.initSocket();
		server.initEpoll();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "Error setting up - " << e.what() << std::endl;
	}
	server.run();

	return (0);
}
