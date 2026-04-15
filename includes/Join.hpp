#ifndef JOIN_HPP
# define JOIN_HPP

# include "Command.hpp"

class Join : public Command {
	public:
		Join(const Message& msg);
		virtual int execute(Client& client, Server& server);
};

#endif
