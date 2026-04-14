#include "../includes/Quit.hpp"

Quit::Quit(const Message& msg) : Command(msg) {}

int Quit::execute(Client& client)
{	
	std::string quitMsg = _msg.params.empty() ? client.getNickname() : _msg.params[0];
	// TODO: Envoyer quitMsg aux autres clients
	return 0;


}
