#ifndef PASS_HPP
# define PASS_HPP

# include "Command.hpp"

class Pass: public Command {
	public:
		Pass(const Message& msg);
		virtual int execute(Client& client);
};

#endif
