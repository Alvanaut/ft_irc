#include "../includes/Nick.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Replies.hpp"

Nick::Nick(const Message& msg) : Command(msg) {}

void Nick::execute(Client& client, Server& server)
{
	const std::string currentNick = client.getNickname().empty() ? "*" : client.getNickname();

	if (_msg.params.empty() || _msg.params[0].empty())
	{
		server.sendToClient(client.getFd(), ERR::noNicknameGiven(currentNick));
		return ;
	}
	const std::string& newNick = _msg.params[0];
	if (!isValidNickname(newNick))
	{
		server.sendToClient(client.getFd(), ERR::erroneusNickname(currentNick, newNick));
		return ;
	}
	if (server.isNickTaken(newNick, client.getFd()))
	{
		server.sendToClient(client.getFd(), ERR::nicknameInUse(currentNick, newNick));
		return ;
	}
	client.setNickname(newNick);
	if (!client.isRegistered() && client.registrationComplete())
	{
		client.setRegistered(true);
		server.sendWelcome(client);
	}
	return ;
}
