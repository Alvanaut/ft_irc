#include "../includes/Server.hpp"
#include "../includes/Command.hpp"
#include "../includes/Pass.hpp"
#include "../includes/Nick.hpp"
#include "../includes/User.hpp"
#include "../includes/Quit.hpp"
#include "../includes/Join.hpp"
#include "../includes/Privmsg.hpp"
#include "../includes/Notice.hpp"
#include "../includes/Part.hpp"
#include "../includes/Topic.hpp"
#include "../includes/Invite.hpp"
#include "../includes/Kick.hpp"
#include "../includes/Mode.hpp"
#include "../includes/Replies.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace
{
	const int	kMaxEvents = 64;
	const int	kBacklog = 128;
}

int g_server_should_run = 1;

Server::Server() : port(0), password(), listen_fd(-1), epoll_fd(-1)
{
}

Server::Server(char *port_arg, char *pass)
	: port(0), password(), listen_fd(-1), epoll_fd(-1)
{
	if (port_arg == NULL || pass == NULL)
		throw std::invalid_argument("Server: null constructor argument");
	port = std::atoi(port_arg);
	if (port <= 0 || port > 65535)
		throw std::invalid_argument("Server: invalid port");
	password = pass;
	initSocket();
	initEpoll();
}

Server::~Server()
{
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
		close(it->first);
	if (listen_fd >= 0)
		close(listen_fd);
	if (epoll_fd >= 0)
		close(epoll_fd);
}

Server::Server(const Server& other)
	: port(other.port), password(other.password), listen_fd(-1), epoll_fd(-1),
	  clients(other.clients), channels(other.channels)
{
}

Server& Server::operator=(const Server &other)
{
	if (this != &other)
	{
		port = other.port;
		password = other.password;
		clients = other.clients;
		channels = other.channels;
		listen_fd = -1;
		epoll_fd = -1;
	}
	return (*this);
}

int Server::setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return (-1);
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		return (-1);
	return ;
}

void Server::initSocket()
{
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
		throw std::runtime_error("Server: socket() failed");
	if (setNonBlocking(listen_fd) == -1)
		throw std::runtime_error("Server: fcntl() failed on listen socket");

	int reuse = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
		throw std::runtime_error("Server: setsockopt() failed");

	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(static_cast<uint16_t>(port));

	if (bind(listen_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
		throw std::runtime_error("Server: bind() failed");
	if (listen(listen_fd, kBacklog) < 0)
		throw std::runtime_error("Server: listen() failed");
}

void Server::initEpoll()
{
	epoll_fd = epoll_create1(0);
	if (epoll_fd < 0)
		throw std::runtime_error("Server: epoll_create1() failed");

	struct epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = listen_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
		throw std::runtime_error("Server: epoll_ctl() failed on listen socket");
}

void Server::removeClientFromAllChannels(int fd)
{
	std::map<int, Client>::iterator client_it = clients.find(fd);
	if (client_it == clients.end())
		return ;

	std::set<std::string> joined = client_it->second.getChannelsJoined();
	for (std::set<std::string>::iterator it = joined.begin(); it != joined.end(); ++it)
	{
		std::map<std::string, Channel>::iterator channel_it = channels.find(*it);
		if (channel_it == channels.end())
			continue ;
		channel_it->second.removeMember(fd);
		if (channel_it->second.empty())
			channels.erase(channel_it);
	}
}

void Server::joinChannel(int fd, const std::string& channel_name)
{
	std::map<int, Client>::iterator client_it = clients.find(fd);
	if (client_it == clients.end())
		return ;

	std::map<std::string, Channel>::iterator channel_it = channels.find(channel_name);
	if (channel_it == channels.end())
	{
		std::pair<std::map<std::string, Channel>::iterator, bool> result =
			channels.insert(std::make_pair(channel_name, Channel(channel_name)));
		channel_it = result.first;
	}
	channel_it->second.addMember(fd);
	client_it->second.joinChannel(channel_name);
}

void Server::disconnectClient(int fd)
{
	removeClientFromAllChannels(fd);
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	clients.erase(fd);
}

void Server::acceptNewClients()
{
	while (true)
	{
		int client_fd = accept(listen_fd, NULL, NULL);
		if (client_fd < 0)
		{
			// means we have nothing more to read
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			std::cerr << "accept() failed" << std::endl;
			break ;
		}
		if (setNonBlocking(client_fd) == -1)
		{
			close(client_fd);
			continue ;
		}

		// add client_fd to listening pool
		struct epoll_event ev;
		std::memset(&ev, 0, sizeof(ev));
		ev.events = EPOLLIN | EPOLLRDHUP;
		ev.data.fd = client_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0)
		{
			close(client_fd);
			continue ;
		}
		std::pair<std::map<int, Client>::iterator, bool> result =
			clients.insert(std::make_pair(client_fd, Client(client_fd)));
		if (!result.second)
		{
			epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
			close(client_fd);
			continue ;
		}

		std::cout << "Accepted client fd=" << client_fd << std::endl;
	}
}

void Server::handleClientEvent(int fd)
{
	char buffer[512];
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it == clients.end())
	{
		disconnectClient(fd);
		return ;
	}
	ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
	if (bytes == 0)
	{
		disconnectClient(fd);
		return ;
	}
	if (bytes < 0)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			disconnectClient(fd);
		return ;
	}
	it->second.appendToInputBuffer(std::string(buffer, bytes));

	while (true)
	{
		const std::string& buf = it->second.getInputBuffer();
		size_t pos = buf.find("\r\n");
		if (pos == std::string::npos)
			break ;
		std::string line = buf.substr(0, pos + 2);
		it->second.eraseFromInputBuffer(pos + 2);
		int result = processCommand(it->second, line);
		if (result == -1)
		{
			disconnectClient(fd);
			return ;
		}
	}
}

void Server::sendToClient(int fd, const std::string& msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}

const std::string& Server::getPassword() const
{
	return (password);
}

std::string Server::getServerName() const
{
	return ("ircserv");
}

bool Server::isNickTaken(const std::string& nick, int exclude_fd) const
{
	for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->first != exclude_fd && it->second.getNickname() == nick)
			return (true);
	}
	return (false);
}

void Server::sendWelcome(Client& client)
{
	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();
	sendToClient(client.getFd(), RPL::welcome(nick, user));
	sendToClient(client.getFd(), RPL::yourHost(nick));
	sendToClient(client.getFd(), RPL::created(nick));
	sendToClient(client.getFd(), RPL::myInfo(nick));
}

void Server::broadcastToClientChannels(const Client& client, const std::string& msg)
{
	const std::set<std::string>& joined = client.getChannelsJoined();
	for (std::set<std::string>::const_iterator it = joined.begin(); it != joined.end(); ++it)
	{
		std::map<std::string, Channel>::iterator ch = channels.find(*it);
		if (ch == channels.end())
			continue ;
		const std::set<int>& members = ch->second.getMembers();
		for (std::set<int>::const_iterator m = members.begin(); m != members.end(); ++m)
		{
			if (*m != client.getFd())
				sendToClient(*m, msg);
		}
	}
}

void Server::broadcastToChannel(const std::string& channel_name, const std::string& msg)
{
	std::map<std::string, Channel>::iterator ch = channels.find(channel_name);
	if (ch == channels.end())
		return ;
	const std::set<int>& members = ch->second.getMembers();
	for (std::set<int>::const_iterator m = members.begin(); m != members.end(); ++m)
		sendToClient(*m, msg);
}

void Server::removeChannel(const std::string& name)
{
	channels.erase(name);
}

Channel* Server::getChannel(const std::string& name)
{
	std::map<std::string, Channel>::iterator it = channels.find(name);
	if (it == channels.end())
		return (NULL);
	return (&it->second);
}

Client* Server::getClientByFd(int fd)
{
	std::map<int, Client>::iterator it = clients.find(fd);
	if (it == clients.end())
		return (NULL);
	return (&it->second);
}

const Client* Server::getClientByFd(int fd) const
{
	std::map<int, Client>::const_iterator it = clients.find(fd);
	if (it == clients.end())
		return (NULL);
	return (&it->second);
}

const Client* Server::getClientByNick(const std::string& nick) const
{
	for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.getNickname() == nick)
			return (&it->second);
	}
	return (NULL);
}

int Server::processCommand(Client& client, const std::string& line)
{
	Message msg = parseMessage(line);
	if (msg.command.empty())
		return ;

	const std::string& nick = client.getNickname().empty() ? std::string("*") : client.getNickname();

	Command* cmd = NULL;
	if (msg.command == "PASS")
		cmd = new Pass(msg);
	else if (msg.command == "NICK")
		cmd = new Nick(msg);
	else if (msg.command == "USER")
		cmd = new User(msg);
	else if (msg.command == "QUIT")
		cmd = new Quit(msg);
	else if (msg.command == "JOIN")
		cmd = new Join(msg);
	else if (msg.command == "PART")
		cmd = new Part(msg);
	else if (msg.command == "PRIVMSG")
		cmd = new Privmsg(msg);
	else if (msg.command == "NOTICE")
		cmd = new Notice(msg);
	else if (msg.command == "TOPIC")
		cmd = new Topic(msg);
	else if (msg.command == "INVITE")
		cmd = new Invite(msg);
	else if (msg.command == "KICK")
		cmd = new Kick(msg);
	else if (msg.command == "MODE")
		cmd = new Mode(msg);
	else if (msg.command == "PING")
	{
		if (msg.params.empty())
			sendToClient(client.getFd(), ":ircserv 409 " + nick + " :No origin specified\r\n");
		else
			sendToClient(client.getFd(), "PONG ircserv :" + msg.params[0] + "\r\n");
		return ;
	}
	else if (msg.command == "PONG")
		return ;

	if (!cmd)
		return ;
	int result = cmd->execute(client, *this);
	delete cmd;
	return (result);
}

void Server::run()
{
	struct epoll_event events[kMaxEvents];

	while (g_server_should_run)
	{
		int ready = epoll_wait(epoll_fd, events, kMaxEvents, -1);
		if (ready < 0)
		{
			if (errno == EINTR)
				continue ;
			throw std::runtime_error("Server: epoll_wait() failed");
		}
		for (int i = 0; i < ready; ++i)
		{
			int fd = events[i].data.fd;
			if (fd == listen_fd)
			{
				acceptNewClients();
				continue ;
			}
			if (events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP))
			{
				disconnectClient(fd);
				continue ;
			}
			if (events[i].events & EPOLLIN)
				handleClientEvent(fd);
		}
	}
}
