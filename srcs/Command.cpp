#include "../includes/Command.hpp"

Command::~Command() {}

Command::Command(const Message& msg) : _msg(msg)
{
}
