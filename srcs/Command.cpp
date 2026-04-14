#include "../includes/Command.hpp"

Command::~Command() {}

Command::Command(const Message& msg) : _msg(msg)
{
}

Message parseMessage(const std::string& message)
{
	std::string cleanMessage = message;
	size_t pos = cleanMessage.find("\r\n");
	if (pos != std::string::npos)
		cleanMessage.erase(pos, 2);
	
	Message msg;
	size_t cursor = 0;
	if (!cleanMessage.empty())
	{
		size_t start = cleanMessage.find(":");
		if (start == 0 && start != std::string::npos)
		{
			size_t end = cleanMessage.find(' ');
			if ( end == std::string::npos)
				return msg;
			msg.prefix = cleanMessage.substr(1, end - 1);
			cursor = end + 1;
		}
		size_t end = cleanMessage.find(' ', cursor);
		if (end == std::string::npos)
		{
			msg.command = cleanMessage.substr(cursor);
			return msg;
		}
		msg.command = cleanMessage.substr(cursor, end - cursor);
		cursor = end + 1;
		while (cursor < cleanMessage.length())
		{
			if (cleanMessage[cursor] == ':')
			{
				msg.params.push_back(cleanMessage.substr(cursor + 1));
				break ;
			}
			size_t newWord = cleanMessage.find(' ', cursor);
			if (newWord == std::string::npos)
			{
				msg.params.push_back(cleanMessage.substr(cursor));
				break ;
			}
			msg.params.push_back(cleanMessage.substr(cursor, newWord - cursor));
			cursor = newWord + 1;
		}
	}
	return msg; 
}

bool isValidNickname(const std::string& name)
{
	if (name.empty() || name.length() > 9)
		return false;
	if (!std::isalpha(name[0]))
		return false;
	size_t i = 1;
	while (i < name.length())
	{
		char c = name[i];
		if (std::isalnum(c) || std::string("-[]\\^{}").find(c) != std::string::npos)
			i++;
		else
			return false;
	}
	return true;
}


bool isValidChannelName(const std::string& name)
{
	if (name.length() > 200 || name.empty())
		return false;
	if (name[0] != '#' && name[0] != '&')
		return false;

	size_t i = 1;
	while (i < name.length())
	{
		char c = name[i];
		if (c == ' ' || c == ',' || c == 7) 
			return false;
		i++;
	}
	return true;
}
