#include "../../includes/commands/User.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Replies.hpp"

User::User(const Message& msg) : Command(msg) {}

void User::execute(Client& client, Server& server)
{
	const std::string nick = client.getNickname().empty() ? "*" : client.getNickname();

	if (_msg.params.size() < 4 || _msg.params[0].empty() || _msg.params[3].empty())
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "USER"));
		return ;
	}
	if (client.isRegistered())
	{
		server.sendToClient(client.getFd(), ERR::alreadyRegistered(nick));
		return ;
	}
	client.setUsername(_msg.params[0]);
	client.setRealName(_msg.params[3]);
	if (client.registrationComplete())
	{
		client.setRegistered(true);
		server.sendWelcome(client);
	}
	return ;
}
