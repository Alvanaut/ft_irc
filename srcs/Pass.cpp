#include "../includes/Pass.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Replies.hpp"

Pass::Pass(const Message& msg) : Command(msg) {}

int Pass::execute(Client& client, Server& server)
{
	const std::string nick = client.getNickname().empty() ? "*" : client.getNickname();

	if (_msg.params.empty())
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "PASS"));
		return (0);
	}
	if (client.isRegistered())
	{
		server.sendToClient(client.getFd(), ERR::alreadyRegistered(nick));
		return (0);
	}
	if (_msg.params[0] != server.getPassword())
	{
		server.sendToClient(client.getFd(), ERR::passwdMismatch(nick));
		return (0);
	}
	client.setPasswordAccepted(true);
	return (0);
}
