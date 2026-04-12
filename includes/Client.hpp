#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>
# include <set>
# include "Message.hpp"
class Client
{
private :
	int 		fd;
	int 		bytes_read;
	char		bytes[512];
	Message		&parsed;

	const std::string username;
	std::string nickname;
	std::string old_nickname;
	std::set<std::string> channels_joined;

	// <OCF>
	Client(const Client& other);
	Client	operator=(const Client &other);
	Client();
	~Client();
	// </OCF>
public :
	Client(std::string username, std::string nickname, std::string real_name);
	void	changeNick(std::string nick);
	void	joinChannel(std::string& channel);
	void	leaveChannel(std::string& channel);
};
#endif // CLIENT_HPP
