#include "../../includes/commands/Quit.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"

Quit::Quit(const Message& msg) : Command(msg) {}

void Quit::execute(Client& client, Server& server)
{
	const std::string quitMsg = _msg.params.empty() ? client.getNickname() : _msg.params[0];
	const std::string nick = client.getNickname().empty() ? "*" : client.getNickname();
	const std::string user = client.getUsername().empty() ? "*" : client.getUsername();

	server.broadcastToClientChannels(client,
		":" + nick + "!" + user + "@ircserv QUIT :" + quitMsg + "\r\n");
	server.sendToClient(client.getFd(),
		"ERROR :Closing Link: " + nick + " (Quit: " + quitMsg + ")\r\n");
	server.disconnectClient(client.getFd());
}
