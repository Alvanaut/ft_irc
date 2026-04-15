#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

Nick::Nick(const Message& msg) : Command(msg) {}

int Nick::execute(Client& client, Server& server)
{
	const std::string currentNick = client.getNickname().empty() ? "*" : client.getNickname();

	if (_msg.params.empty() || _msg.params[0].empty())
	{
		server.sendToClient(client.getFd(), ":ircserv 431 " + currentNick + " :No nickname given\r\n");
		return (0);
	}
	const std::string& newNick = _msg.params[0];
	if (!isValidNickname(newNick))
	{
		server.sendToClient(client.getFd(), ":ircserv 432 " + currentNick + " " + newNick + " :Erroneous nickname\r\n");
		return (0);
	}
	if (server.isNickTaken(newNick, client.getFd()))
	{
		server.sendToClient(client.getFd(), ":ircserv 433 " + currentNick + " " + newNick + " :Nickname is already in use\r\n");
		return (0);
	}
	client.setNickname(newNick);
	if (!client.isRegistered() && client.registrationComplete())
	{
		client.setRegistered(true);
		server.sendWelcome(client);
	}
	return (0);
}
