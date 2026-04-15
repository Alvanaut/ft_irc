#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include "Command.hpp"

class Privmsg : public Command {
	public:
		Privmsg(const Message& msg);
		virtual int execute(Client& client, Server& server);
};

#endif
