#ifndef PASS_HPP
# define PASS_HPP

# include "Command.hpp"

class Pass: public Command {
	public:
		Pass(const Message& msg);
		void	execute(Client& client, Server& server);
};

#endif
