#include "../includes/Client.hpp"

Client::Client() {}

Client::~Client() {}

Client::Client(int fd) : _fd(fd), _authenticated(false), _registered(false) {}

void        Client::setBuffer(const std::string& buffer) { _buffer = buffer;}
void        Client::setUsername(const std::string& username) { _username = username; }
void        Client::setNickname(const std::string& nickname) { _nickname = nickname; }
void        Client::setRealname(const std::string& realname) { _realname = realname; }
void        Client::setRegistered(bool registered) { _registered = registered; }
void        Client::setAuthenticated(bool authenticated) { _authenticated = authenticated; }

int         Client::getFd() const { return _fd; }
std::string Client::getBuffer() const { return _buffer; }
std::string Client::getUsername() const { return _username; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getRealname() const { return _realname; }
bool        Client::getRegistered() const { return _registered; }
bool        Client::getAuthenticated() const { return _authenticated; }

void        Client::appendToBuffer(const std::string& data)
{
    _buffer.append(data);
}
std::string Client::extractMessage()
{
    size_t end = _buffer.find("\r\n");
    if (end == std::string::npos)
        return "";
    std::string message = _buffer.substr(0, end );
    _buffer.erase(0, end + 2);
    return (message);
}