#include "../includes/Privmsg.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Replies.hpp"

Privmsg::Privmsg(const Message& msg) : Command(msg) {}

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

static void sendToTarget(Client& client, Server& server,
	const std::string& target, const std::string& text, bool withErrors)
{
	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();
	const std::string  msg  = ":" + nick + "!" + user + "@ircserv PRIVMSG "
		+ target + " :" + text + "\r\n";

	if (target[0] == '#' || target[0] == '&')
	{
		Channel* ch = server.getChannel(target);
		if (!ch)
		{
			if (withErrors)
				server.sendToClient(client.getFd(), ERR::noSuchChannel(nick, target));
			return ;
		}
		if (!ch->hasMember(client.getFd()))
		{
			if (withErrors)
				server.sendToClient(client.getFd(), ERR::cannotSendToChan(nick, target));
			return ;
		}
		// Envoyer à tous les membres sauf l'expéditeur
		const std::set<int>& members = ch->getMembers();
		for (std::set<int>::const_iterator it = members.begin(); it != members.end(); ++it)
		{
			if (*it != client.getFd())
				server.sendToClient(*it, msg);
		}
	}
	else
	{
		const Client* dest = server.getClientByNick(target);
		if (!dest)
		{
			if (withErrors)
				server.sendToClient(client.getFd(), ERR::noSuchNick(nick, target));
			return ;
		}
		server.sendToClient(dest->getFd(), msg);
	}
}

void Privmsg::execute(Client& client, Server& server)
{
	const std::string& nick = client.getNickname();

	if (_msg.params.empty() || _msg.params[0].empty())
	{
		server.sendToClient(client.getFd(), ERR::noRecipient(nick, "PRIVMSG"));
		return ;
	}
	if (_msg.params.size() < 2 || _msg.params[1].empty())
	{
		server.sendToClient(client.getFd(), ERR::noTextToSend(nick));
		return ;
	}

	std::vector<std::string> targets = split(_msg.params[0], ',');
	const std::string& text = _msg.params[1];

	for (size_t i = 0; i < targets.size(); ++i)
		sendToTarget(client, server, targets[i], text, true);
	return ;
}
