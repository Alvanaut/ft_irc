#include "../includes/Channel.hpp"

Channel::Channel() : name(), members()
{
}

Channel::Channel(const std::string& channel_name)
	: name(channel_name), members()
{
}

Channel::Channel(const Channel& other)
	: name(other.name), members(other.members)
{
}

Channel& Channel::operator=(const Channel& other)
{
	if (this != &other)
	{
		name = other.name;
		members = other.members;
	}
	return (*this);
}

Channel::~Channel()
{
}

const std::string& Channel::getName() const
{
	return (name);
}

const std::set<int>& Channel::getMembers() const
{
	return (members);
}

bool Channel::hasMember(int fd) const
{
	return (members.find(fd) != members.end());
}

bool Channel::empty() const
{
	return (members.empty());
}

void Channel::addMember(int fd)
{
	members.insert(fd);
}

void Channel::removeMember(int fd)
{
	members.erase(fd);
}
