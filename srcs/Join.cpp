#include "../includes/Join.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Replies.hpp"

Join::Join(const Message& msg) : Command(msg) {}

static std::vector<std::string> split(const std::string& s, char delim)
{
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = s.find(delim);
	while (end != std::string::npos)
	{
		if (end > start)
			tokens.push_back(s.substr(start, end - start));
		start = end + 1;
		end = s.find(delim, start);
	}
	if (start < s.size())
		tokens.push_back(s.substr(start));
	return (tokens);
}

static std::string buildNamesList(const Channel& ch, const Server& server)
{
	std::string names;
	const std::set<int>& members = ch.getMembers();
	for (std::set<int>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		const Client* c = server.getClientByFd(*it);
		if (!c)
			continue ;
		if (!names.empty())
			names += " ";
		if (ch.isOperator(*it))
			names += "@";
		names += c->getNickname();
	}
	return (names);
}

static void joinOne(Client& client, Server& server,
					const std::string& chanName, const std::string& key)
{
	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();
	const std::string  prefix = ":" + nick + "!" + user + "@ircserv";

	if (!isValidChannelName(chanName))
	{
		server.sendToClient(client.getFd(), ERR::badChanMask(nick, chanName));
		return ;
	}
	if (client.isInChannel(chanName))
		return ;

	Channel* ch = server.getChannel(chanName);
	if (ch)
	{
		if (ch->isInviteOnly() && !ch->isInvited(client.getFd()))
		{
			server.sendToClient(client.getFd(), ERR::inviteOnlyChan(nick, chanName));
			return ;
		}
		if (!ch->getKey().empty() && ch->getKey() != key)
		{
			server.sendToClient(client.getFd(), ERR::badChannelKey(nick, chanName));
			return ;
		}
		if (ch->getUserLimit() > 0 && (int)ch->getMembers().size() >= ch->getUserLimit())
		{
			server.sendToClient(client.getFd(), ERR::channelIsFull(nick, chanName));
			return ;
		}
	}

	bool isNew = (ch == NULL);
	server.joinChannel(client.getFd(), chanName);
	ch = server.getChannel(chanName);

	if (isNew)
		ch->addOperator(client.getFd());
	if (ch->isInvited(client.getFd()))
		ch->removeInvited(client.getFd());

	server.broadcastToChannel(chanName, prefix + " JOIN :" + chanName + "\r\n");

	if (ch->getTopic().empty())
		server.sendToClient(client.getFd(), RPL::noTopic(nick, chanName));
	else
		server.sendToClient(client.getFd(), RPL::topic(nick, chanName, ch->getTopic()));

	server.sendToClient(client.getFd(), RPL::namReply(nick, chanName, buildNamesList(*ch, server)));
	server.sendToClient(client.getFd(), RPL::endOfNames(nick, chanName));
}

void Join::execute(Client& client, Server& server)
{
	const std::string nick = client.getNickname();

	if (_msg.params.empty() || _msg.params[0].empty())
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "JOIN"));
		return ;
	}

	std::vector<std::string> chans = split(_msg.params[0], ',');
	std::vector<std::string> keys;
	if (_msg.params.size() > 1)
		keys = split(_msg.params[1], ',');

	for (size_t i = 0; i < chans.size(); ++i)
	{
		const std::string key = (i < keys.size()) ? keys[i] : "";
		joinOne(client, server, chans[i], key);
	}
	return ;
}
