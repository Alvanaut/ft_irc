#ifndef KICK_HPP
# define KICK_HPP

# include "../Command.hpp"

class Kick : public Command {
	public:
		Kick(const Message& msg);
		void	execute(Client& client, Server& server);
};

#endif
