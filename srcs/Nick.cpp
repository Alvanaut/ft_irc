#include "../includes/Nick.hpp"

Nick::Nick(const Message& msg) : Command(msg)
{
}

int Nick::execute(Client& client)
{
	if (_msg.params.empty())
		return 1;
	if (!isValidNickname(_msg.params[0]))
		return 1;
	// client.setNickname();
	return 0;
}
