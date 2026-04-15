#include "../../includes/commands/Pass.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Replies.hpp"

Pass::Pass(const Message& msg) : Command(msg) {}

void Pass::execute(Client& client, Server& server)
{
	const std::string nick = client.getNickname().empty() ? "*" : client.getNickname();

	if (_msg.params.empty())
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "PASS"));
		return ;
	}
	if (client.isRegistered())
	{
		server.sendToClient(client.getFd(), ERR::alreadyRegistered(nick));
		return ;
	}
	if (_msg.params[0] != server.getPassword())
	{
		server.sendToClient(client.getFd(), ERR::passwdMismatch(nick));
		return ;
	}
	client.setPasswordAccepted(true);
	return ;
}
