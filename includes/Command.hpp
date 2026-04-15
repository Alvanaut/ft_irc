#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <vector>
# include <string>

struct Message {
	std::string prefix; // Optionnel
	std::string command;
	std::vector<std::string> params;
};

Message parseMessage(const std::string& message);
bool isValidNickname(const std::string& name);
bool isValidChannelName(const std::string& name);

class Client;
class Server;

class Command {
	protected:
		Message _msg;
	public:
		virtual ~Command();
		Command(const Message& msg);

		virtual void	execute(Client& client, Server& server) = 0;

};
#endif
