#ifndef USER_HPP
# define USER_HPP

# include "../Command.hpp"

class User : public Command {
	public:
		User(const Message& msg);
		void	execute(Client& client, Server& server);
};

#endif
