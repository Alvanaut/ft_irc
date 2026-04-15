#ifndef PART_HPP
# define PART_HPP

# include "../Command.hpp"

class Part : public Command {
	public:
		Part(const Message& msg);
		void	execute(Client& client, Server& server);
};

#endif
