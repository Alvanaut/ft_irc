#include "../../includes/commands/Notice.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Notice::Notice(const Message& msg) : Command(msg) {}

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

// NOTICE : même logique que PRIVMSG mais sans aucune réponse d'erreur
void Notice::execute(Client& client, Server& server)
{
	if (_msg.params.size() < 2 || _msg.params[0].empty() || _msg.params[1].empty())
		return ;

	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();
	const std::string& text = _msg.params[1];

	std::vector<std::string> targets = split(_msg.params[0], ',');

	for (size_t i = 0; i < targets.size(); ++i)
	{
		const std::string& target = targets[i];
		const std::string  msg = ":" + nick + "!" + user + "@ircserv NOTICE "
			+ target + " :" + text + "\r\n";

		if (target[0] == '#' || target[0] == '&')
		{
			Channel* ch = server.getChannel(target);
			if (!ch || !ch->hasMember(client.getFd()))
				continue ;
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
			if (dest)
				server.sendToClient(dest->getFd(), msg);
		}
	}
	return ;
}
