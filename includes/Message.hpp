#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <iostream>
# include <vector>
# include <string>

struct Message {
	std::string prefix; // Optionnel
	std::string command;
	std::vector<std::string> params;
};

Message parseMessage(const std::string& message);

#endif
