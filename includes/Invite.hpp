#ifndef INVITE_HPP
# define INVITE_HPP

# include "Command.hpp"

class Invite : public Command {
	public:
		Invite(const Message& msg);
		virtual int execute(Client& client, Server& server);
};

#endif
