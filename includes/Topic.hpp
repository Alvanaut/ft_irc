#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "Command.hpp"

class Topic : public Command {
	public:
		Topic(const Message& msg);
		virtual int execute(Client& client, Server& server);
};

#endif
