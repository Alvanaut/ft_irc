#include "../includes/Pass.hpp"

Pass::Pass(const Message& msg) : Command(msg) {}

int Pass::execute(Client& client)
{
	if (_msg.params.empty())
		return 1;
	// client.setPassword(_msg.params[0]);
	return 0;
}
