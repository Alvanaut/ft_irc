#include "../includes/Client.hpp"

Client::Client()
	: fd(-1), input_buffer(), password_accepted(false), has_nickname(false),
	  has_username(false), registered(false), nickname(), old_nickname(),
	  username(), real_name(), channels_joined()
{
}

Client::Client(int client_fd)
	: fd(client_fd), input_buffer(), password_accepted(false), has_nickname(false),
	  has_username(false), registered(false), nickname(), old_nickname(),
	  username(), real_name(), channels_joined()
{
}

Client::Client(const Client& other)
	: fd(other.fd), input_buffer(other.input_buffer),
	  password_accepted(other.password_accepted),
	  has_nickname(other.has_nickname), has_username(other.has_username),
	  registered(other.registered), nickname(other.nickname),
	  old_nickname(other.old_nickname), username(other.username),
	  real_name(other.real_name), channels_joined(other.channels_joined)
{
}

Client& Client::operator=(const Client& other)
{
	if (this != &other)
	{
		fd = other.fd;
		input_buffer = other.input_buffer;
		password_accepted = other.password_accepted;
		has_nickname = other.has_nickname;
		has_username = other.has_username;
		registered = other.registered;
		nickname = other.nickname;
		old_nickname = other.old_nickname;
		username = other.username;
		real_name = other.real_name;
		channels_joined = other.channels_joined;
	}
	return (*this);
}

Client::~Client()
{
}

int Client::getFd() const
{
	return (fd);
}

const std::string& Client::getInputBuffer() const
{
	return (input_buffer);
}

bool Client::isPasswordAccepted() const
{
	return (password_accepted);
}

bool Client::hasNickname() const
{
	return (has_nickname);
}

bool Client::hasUsername() const
{
	return (has_username);
}
bool Client::isRegistered() const
{
	return (registered);
}

const std::string& Client::getNickname() const
{
	return (nickname);
}

const std::string& Client::getOldNickname() const
{
	return (old_nickname);
}

const std::string& Client::getUsername() const
{
	return (username);
}

const std::string& Client::getRealName() const
{
	return (real_name);
}

const std::set<std::string>& Client::getChannelsJoined() const
{
	return (channels_joined);
}

void Client::setFd(int new_fd)
{
	fd = new_fd;
}

void Client::appendToInputBuffer(const std::string& data)
{
	input_buffer += data;
}

void Client::clearInputBuffer()
{
	input_buffer.clear();
}

void Client::eraseFromInputBuffer(size_t count)
{
	if (count >= input_buffer.size())
	{
		input_buffer.clear();
		return ;
	}
	input_buffer.erase(0, count);
}

void Client::setPasswordAccepted(bool accepted)
{
	password_accepted = accepted;
}

void Client::setNickname(const std::string& new_nickname)
{
	old_nickname = nickname;
	nickname = new_nickname;
	has_nickname = !nickname.empty();
}

void Client::setUsername(const std::string& new_username)
{
	username = new_username;
	has_username = !username.empty();
}

void Client::setRealName(const std::string& new_real_name)
{
	real_name = new_real_name;
}

void Client::setRegistered(bool value)
{
	registered = value;
}

void Client::joinChannel(const std::string& channel)
{
	channels_joined.insert(channel);
}

void Client::leaveChannel(const std::string& channel)
{
	channels_joined.erase(channel);
}

bool Client::isInChannel(const std::string& channel) const
{
	return (channels_joined.find(channel) != channels_joined.end());
}

bool Client::registrationComplete() const
{
	return (password_accepted && has_nickname && has_username);
}
