#include "../../includes/commands/Part.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Replies.hpp"

Part::Part(const Message& msg) : Command(msg) {}

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
