#ifndef QUIT_HPP
# define QUIT_HPP

# include "Command.hpp"

class Quit : public Command {
	public:
		Quit(const Message& msg);
		virtual int execute(Client& client, Server& server);
};

#endif
