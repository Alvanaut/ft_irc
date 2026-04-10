#ifndef SERVER_HPP
# define SERVER_HPP
# include "Client.hpp"
# include "Channel.hpp"
# include "<map>"

class Server
{
private :
	int								port;
	std::string						password;
	
	std::map<int, Client>			clients;  // key = socket fd
	std::map<std::string, Channel>	channels; // key = channel name;

	// <OCF>
	Server(const Server& other);
	Server	operator=(const Server &other);
public :
	Server();
	~Server();
	// </OCF>

	Server(char *port, char *pass);
	int run();
};

#endif // SERVER_HPP
