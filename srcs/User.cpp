#include "../includes/User.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

User::User(const Message& msg) : Command(msg) {}

int User::execute(Client& client, Server& server)
{
	const std::string nick = client.getNickname().empty() ? "*" : client.getNickname();

	if (_msg.params.size() < 4 || _msg.params[0].empty() || _msg.params[3].empty())
	{
		server.sendToClient(client.getFd(), ":ircserv 461 " + nick + " USER :Not enough parameters\r\n");
		return (0);
	}
	if (client.isRegistered())
	{
		server.sendToClient(client.getFd(), ":ircserv 462 " + nick + " :You may not reregister\r\n");
		return (0);
	}
	client.setUsername(_msg.params[0]);
	client.setRealName(_msg.params[3]);
	if (client.registrationComplete())
	{
		client.setRegistered(true);
		server.sendWelcome(client);
	}
	return (0);
}
