#include "../../includes/commands/Invite.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Replies.hpp"

Invite::Invite(const Message& msg) : Command(msg) {}

void Invite::execute(Client& client, Server& server)
{
	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();

	if (_msg.params.size() < 2)
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "INVITE"));
		return ;
	}

	const std::string& targetNick = _msg.params[0];
	const std::string& chanName   = _msg.params[1];

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
	if (ch->isInviteOnly() && !ch->isOperator(client.getFd()))
	{
		server.sendToClient(client.getFd(), ERR::chanopPrivsNeeded(nick, chanName));
		return ;
	}

	const Client* target = server.getClientByNick(targetNick);
	if (!target)
	{
		server.sendToClient(client.getFd(), ERR::noSuchNick(nick, targetNick));
		return ;
	}
	if (ch->hasMember(target->getFd()))
	{
		server.sendToClient(client.getFd(), ERR::userOnChannel(nick, targetNick, chanName));
		return ;
	}

	ch->addInvited(target->getFd());

	server.sendToClient(client.getFd(), RPL::inviting(nick, targetNick, chanName));
	server.sendToClient(target->getFd(),
		":" + nick + "!" + user + "@ircserv INVITE " + targetNick + " :" + chanName + "\r\n");
	return ;
}
