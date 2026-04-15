#ifndef NICK_HPP
# define NICK_HPP

# include "Command.hpp"

class Nick: public Command {
	public:
		Nick(const Message& msg);
		void	execute(Client& client, Server& server);
};

#endif
