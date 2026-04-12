#ifndef SERVER_HPP
# define SERVER_HPP
# include "Client.hpp"
# include "Channel.hpp"
# include <map>
# include <string>
# include <sys/epoll.h>

class Server
{
private :
	int								port;
	std::string						password;
	int							listen_fd;
	int							epoll_fd;

	std::map<int, Client>			clients;  // key = socket fd
	std::map<std::string, Channel>	channels; // key = channel name

	// <OCF>
	Server(const Server& other);
	Server&	operator=(const Server &other);
	void	initSocket();
	void	initEpoll();
	void	acceptNewClients();
	void	handleClientEvent(int fd);
	void	disconnectClient(int fd);
	int		setNonBlocking(int fd);
public :
	Server();
	Server(char *port, char *pass);
	~Server();
	void run();
};

#endif // SERVER_HPP
