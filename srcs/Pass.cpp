#include "../includes/Pass.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

Pass::Pass(const Message& msg) : Command(msg) {}

int Pass::execute(Client& client, Server& server)
{
	const std::string nick = client.getNickname().empty() ? "*" : client.getNickname();

	if (_msg.params.empty())
	{
		server.sendToClient(client.getFd(), ":ircserv 461 " + nick + " PASS :Not enough parameters\r\n");
		return (0);
	}
	if (client.isRegistered())
	{
		server.sendToClient(client.getFd(), ":ircserv 462 " + nick + " :You may not reregister\r\n");
		return (0);
	}
	if (_msg.params[0] != server.getPassword())
	{
		server.sendToClient(client.getFd(), ":ircserv 464 " + nick + " :Password incorrect\r\n");
		return (0);
	}
	client.setPasswordAccepted(true);
	return (0);
}
