#include "../includes/Join.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

Join::Join(const Message& msg) : Command(msg) {}

// Split une string par un délimiteur
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

// Construit la liste des noms du channel avec @ pour les ops
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
	const std::string nick = client.getNickname();
	const std::string user = client.getUsername();
	const std::string prefix = ":" + nick + "!" + user + "@ircserv";

	if (!isValidChannelName(chanName))
	{
		server.sendToClient(client.getFd(), ":ircserv 476 " + nick + " " + chanName + " :Bad Channel Mask\r\n");
		return ;
	}

	// Déjà dans le channel → ignorer
	if (client.isInChannel(chanName))
		return ;

	Channel* ch = server.getChannel(chanName);

	if (ch)
	{
		// Vérifications seulement si le channel existe déjà
		if (ch->isInviteOnly() && !ch->isInvited(client.getFd()))
		{
			server.sendToClient(client.getFd(), ":ircserv 473 " + nick + " " + chanName + " :Cannot join channel (+i)\r\n");
			return ;
		}
		if (!ch->getKey().empty() && ch->getKey() != key)
		{
			server.sendToClient(client.getFd(), ":ircserv 475 " + nick + " " + chanName + " :Cannot join channel (+k)\r\n");
			return ;
		}
		if (ch->getUserLimit() > 0 && (int)ch->getMembers().size() >= ch->getUserLimit())
		{
			server.sendToClient(client.getFd(), ":ircserv 471 " + nick + " " + chanName + " :Cannot join channel (+l)\r\n");
			return ;
		}
	}

	bool isNew = (ch == NULL);
	server.joinChannel(client.getFd(), chanName);         // crée si besoin + addMember
	ch = server.getChannel(chanName);

	if (isNew)
		ch->addOperator(client.getFd());                  // premier arrivé = op
	if (ch->isInvited(client.getFd()))
		ch->removeInvited(client.getFd());

	// Broadcast JOIN à tout le channel (y compris le joiner)
	server.broadcastToChannel(chanName, prefix + " JOIN :" + chanName + "\r\n");

	// Topic
	if (ch->getTopic().empty())
		server.sendToClient(client.getFd(), ":ircserv 331 " + nick + " " + chanName + " :No topic is set\r\n");
	else
		server.sendToClient(client.getFd(), ":ircserv 332 " + nick + " " + chanName + " :" + ch->getTopic() + "\r\n");

	// NAMES
	server.sendToClient(client.getFd(), ":ircserv 353 " + nick + " = " + chanName + " :" + buildNamesList(*ch, server) + "\r\n");
	server.sendToClient(client.getFd(), ":ircserv 366 " + nick + " " + chanName + " :End of /NAMES list\r\n");
}

int Join::execute(Client& client, Server& server)
{
	const std::string nick = client.getNickname();

	if (_msg.params.empty() || _msg.params[0].empty())
	{
		server.sendToClient(client.getFd(), ":ircserv 461 " + nick + " JOIN :Not enough parameters\r\n");
		return (0);
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
	return (0);
}
