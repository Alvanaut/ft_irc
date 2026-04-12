#include "Server.hpp"

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
	return (0);
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

void Server::disconnectClient(int fd)
{
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

		struct epoll_event ev;
		std::memset(&ev, 0, sizeof(ev));
		ev.events = EPOLLIN | EPOLLRDHUP;
		ev.data.fd = client_fd;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0)
		{
			close(client_fd);
			continue ;
		}

		std::cout << "Accepted client fd=" << client_fd << std::endl;
	}
}

void Server::handleClientEvent(int fd)
{
	char buffer[512];
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
	std::cout << "Received " << bytes << " bytes from fd=" << fd << std::endl;
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
