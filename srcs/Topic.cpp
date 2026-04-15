#include "../includes/Topic.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Replies.hpp"

Topic::Topic(const Message& msg) : Command(msg) {}

int Topic::execute(Client& client, Server& server)
{
	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();

	if (_msg.params.empty())
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "TOPIC"));
		return (0);
	}

	const std::string& chanName = _msg.params[0];
	Channel* ch = server.getChannel(chanName);

	if (!ch)
	{
		server.sendToClient(client.getFd(), ERR::noSuchChannel(nick, chanName));
		return (0);
	}
	if (!ch->hasMember(client.getFd()))
	{
		server.sendToClient(client.getFd(), ERR::notOnChannel(nick, chanName));
		return (0);
	}

	// Lecture du topic (pas de 2e param)
	if (_msg.params.size() < 2)
	{
		if (ch->getTopic().empty())
			server.sendToClient(client.getFd(), RPL::noTopic(nick, chanName));
		else
			server.sendToClient(client.getFd(), RPL::topic(nick, chanName, ch->getTopic()));
		return (0);
	}

	// Modification du topic
	if (ch->isTopicRestricted() && !ch->isOperator(client.getFd()))
	{
		server.sendToClient(client.getFd(), ERR::chanopPrivsNeeded(nick, chanName));
		return (0);
	}

	ch->setTopic(_msg.params[1]);

	const std::string broadcast = ":" + nick + "!" + user + "@ircserv TOPIC "
		+ chanName + " :" + _msg.params[1] + "\r\n";
	server.broadcastToChannel(chanName, broadcast);
	return (0);
}
