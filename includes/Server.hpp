#ifndef SERVER_HPP
# define SERVER_HPP
# include "Channel.hpp"
# include "Client.hpp"
# include <map>
# include <string>
# include <sys/epoll.h>

class Server
{
private :
	int								port;
	std::string						password;
	int								listen_fd;
	int								epoll_fd;

	std::map<int, Client>			clients;  // key = socket fd
	std::map<std::string, Channel>	channels; // key = channel name

	// <OCF>
	Server(const Server& other);
	Server&	operator=(const Server &other);
	
	void	acceptNewClients();
	void	handleClientEvent(int fd);
	void	removeClientFromAllChannels(int fd);
	void	unexpectedDisconnect(int fd, std::string reason);
	void	cleanup();
	Server();

public :
	Server(char *port, char *pass);
	~Server();
	void run();

	// This is only so that the QUIT command can access it
	void				disconnectClient(int fd);
	void				unmarkClientForOutput(int fd);
	void				markClientForOutput(int fd);
	void				addToClientOutput(int fd, std::string &msg);
	void				sendClientOutput(int fd);
	void				initSocket();
	void				initEpoll();
	void				joinChannel(int fd, const std::string& channel_name);
	void				addToClientOutput(int fd, const std::string& msg);
	const std::string&	getPassword() const;
	std::string			getServerName() const;
	bool				isNickTaken(const std::string& nick, int exclude_fd) const;
	void				sendWelcome(Client& client);
	void				broadcastToClientChannels(const Client& client, const std::string& msg);
	void				broadcastToChannel(const std::string& channel_name, const std::string& msg);
	Channel*			getChannel(const std::string& name);
	void				removeChannel(const std::string& name);
	Client*				getClientByFd(int fd);
	const Client*		getClientByFd(int fd) const;
	const Client*		getClientByNick(const std::string& nick) const;
	void				processCommand(Client& client, const std::string& line);
};

#endif // SERVER_HPP
