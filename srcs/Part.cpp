#include "../includes/Part.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Replies.hpp"

Part::Part(const Message& msg) : Command(msg) {}

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

void Part::execute(Client& client, Server& server)
{
	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();

	if (_msg.params.empty() || _msg.params[0].empty())
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "PART"));
		return ;
	}

	const std::string partMsg = (_msg.params.size() > 1 && !_msg.params[1].empty())
		? _msg.params[1] : nick;

	std::vector<std::string> chans = split(_msg.params[0], ',');

	for (size_t i = 0; i < chans.size(); ++i)
	{
		const std::string& chanName = chans[i];
		Channel* ch = server.getChannel(chanName);

		if (!ch)
		{
			server.sendToClient(client.getFd(), ERR::noSuchChannel(nick, chanName));
			continue ;
		}
		if (!ch->hasMember(client.getFd()))
		{
			server.sendToClient(client.getFd(), ERR::notOnChannel(nick, chanName));
			continue ;
		}

		// Broadcast PART à tous les membres (y compris le partant)
		const std::string partBroadcast = ":" + nick + "!" + user + "@ircserv PART "
			+ chanName + " :" + partMsg + "\r\n";
		server.broadcastToChannel(chanName, partBroadcast);

		// Retirer le client du channel
		ch->removeMember(client.getFd());
		client.leaveChannel(chanName);

		// Supprimer le channel s'il est vide
		if (ch->empty())
			server.removeChannel(chanName);
	}
	return ;
}
