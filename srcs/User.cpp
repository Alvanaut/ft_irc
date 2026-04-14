#include "../includes/User.hpp"

User::User(const Message& msg) : Command(msg) {}

int User::execute(Client& client)
{
	if (_msg.params.size() < 4 || _msg.params[0].empty() || _msg.params[3].empty())
		return 1;
	client.setUsername(_msg.params[0]);
	client.setRealname(_msg.params[3]);
	return 0;
}
