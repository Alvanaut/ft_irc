#include "Server.hpp"

int g_server_should_run = 1;




int Server::run()
{
	while (g_server_should_run)
	{
		
	}
}

// ----------BEGIN ORTHODOX CANONICAL FORM----------
// 
Server::Server() {}
Server::Server(const Server& other) {(void) other;}
Server::~Server() {}

Server	Server::operator=(const Server &other)
{ (void) other; return (*this); }
// ----------END ORTHODOX CANONICAL FORM----------

