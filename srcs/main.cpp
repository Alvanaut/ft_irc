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
	

	try
	{
		Server	server(av[1], av[2]);
		server.initSocket();
		server.initEpoll();
		server.run();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "Runtime error, exiting: " << e.what() << std::endl;
		return (1);
	}
	catch (std::invalid_argument &e)
	{
		std::cerr << "Invalid argument: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}
