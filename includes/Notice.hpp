#ifndef NOTICE_HPP
# define NOTICE_HPP

# include "Command.hpp"

class Notice : public Command {
	public:
		Notice(const Message& msg);
		virtual int execute(Client& client, Server& server);
};

#endif
