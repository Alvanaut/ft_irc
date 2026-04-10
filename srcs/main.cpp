#include "../includes/Message.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "Invalid format: Prefix -> commands -> params" << std::endl;
		return 1;
	}
	Message message = parseMessage(av[1]);
	std::cout << "Prefix : " << message.prefix << std::endl;
	std::cout << "Commands : " << message.command << std::endl;
	for (size_t i = 0; i < message.params.size(); i++)
		std::cout << "Params [" << i << "] : " << message.params[i] << std::endl;
	return 0;
}
