#ifndef PART_HPP
# define PART_HPP

# include "Command.hpp"

class Part : public Command {
	public:
		Part(const Message& msg);
		virtual int execute(Client& client, Server& server);
};

#endif
