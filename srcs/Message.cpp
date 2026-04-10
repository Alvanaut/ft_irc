#include "../includes/Message.hpp"

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
