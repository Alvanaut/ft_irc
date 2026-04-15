#ifndef MODE_HPP
# define MODE_HPP

# include "../Command.hpp"

class Mode : public Command {
	public:
		Mode(const Message& msg);
		void	execute(Client& client, Server& server);
};

#endif
