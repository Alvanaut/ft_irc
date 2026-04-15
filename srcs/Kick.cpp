#include "../includes/Kick.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Replies.hpp"

Kick::Kick(const Message& msg) : Command(msg) {}

void Kick::execute(Client& client, Server& server)
{
	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();

	if (_msg.params.size() < 2)
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "KICK"));
		return ;
	}

	const std::string& chanName   = _msg.params[0];
	const std::string& targetNick = _msg.params[1];
	const std::string  reason     = (_msg.params.size() > 2 && !_msg.params[2].empty())
		? _msg.params[2] : nick;

	Channel* ch = server.getChannel(chanName);
	if (!ch)
	{
		server.sendToClient(client.getFd(), ERR::noSuchChannel(nick, chanName));
		return ;
	}
	if (!ch->hasMember(client.getFd()))
	{
		server.sendToClient(client.getFd(), ERR::notOnChannel(nick, chanName));
		return ;
	}
	if (!ch->isOperator(client.getFd()))
	{
		server.sendToClient(client.getFd(), ERR::chanopPrivsNeeded(nick, chanName));
		return ;
	}

	const Client* target = server.getClientByNick(targetNick);
	if (!target || !ch->hasMember(target->getFd()))
	{
		server.sendToClient(client.getFd(), ERR::userNotInChannel(nick, targetNick, chanName));
		return ;
	}

	const std::string broadcast = ":" + nick + "!" + user + "@ircserv KICK "
		+ chanName + " " + targetNick + " :" + reason + "\r\n";
	server.broadcastToChannel(chanName, broadcast);

	int targetFd = target->getFd();
	ch->removeMember(targetFd);

	Client* targetClient = server.getClientByFd(targetFd);
	if (targetClient)
		targetClient->leaveChannel(chanName);

	if (ch->empty())
		server.removeChannel(chanName);

	return ;
}
