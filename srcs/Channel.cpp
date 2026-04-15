#include "../includes/Channel.hpp"

Channel::Channel()
	: name(), members(), operators(), invited(), topic(), key(),
	  invite_only(false), topic_restricted(false), user_limit(0)
{
}

Channel::Channel(const std::string& channel_name)
	: name(channel_name), members(), operators(), invited(), topic(), key(),
	  invite_only(false), topic_restricted(false), user_limit(0)
{
}

Channel::Channel(const Channel& other)
	: name(other.name), members(other.members), operators(other.operators),
	  invited(other.invited), topic(other.topic), key(other.key),
	  invite_only(other.invite_only), topic_restricted(other.topic_restricted),
	  user_limit(other.user_limit)
{
}

Channel& Channel::operator=(const Channel& other)
{
	if (this != &other)
	{
		name             = other.name;
		members          = other.members;
		operators        = other.operators;
		invited          = other.invited;
		topic            = other.topic;
		key              = other.key;
		invite_only      = other.invite_only;
		topic_restricted = other.topic_restricted;
		user_limit       = other.user_limit;
	}
	return (*this);
}

Channel::~Channel() {}

const std::string& Channel::getName() const        { return (name); }
const std::set<int>& Channel::getMembers() const   { return (members); }
const std::set<int>& Channel::getOperators() const { return (operators); }
const std::string& Channel::getTopic() const       { return (topic); }
const std::string& Channel::getKey() const         { return (key); }
bool Channel::isInviteOnly() const                 { return (invite_only); }
bool Channel::isTopicRestricted() const            { return (topic_restricted); }
int  Channel::getUserLimit() const                 { return (user_limit); }

void Channel::setTopic(const std::string& t)   { topic = t; }
void Channel::setKey(const std::string& k)     { key = k; }
void Channel::setInviteOnly(bool value)        { invite_only = value; }
void Channel::setTopicRestricted(bool value)   { topic_restricted = value; }
void Channel::setUserLimit(int limit)          { user_limit = limit; }

bool Channel::hasMember(int fd) const  { return (members.find(fd) != members.end()); }
bool Channel::isOperator(int fd) const { return (operators.find(fd) != operators.end()); }
bool Channel::isInvited(int fd) const  { return (invited.find(fd) != invited.end()); }
bool Channel::empty() const            { return (members.empty()); }

void Channel::addMember(int fd)     { members.insert(fd); }
void Channel::removeMember(int fd)  { members.erase(fd); operators.erase(fd); }
void Channel::addOperator(int fd)   { operators.insert(fd); }
void Channel::removeOperator(int fd){ operators.erase(fd); }
void Channel::addInvited(int fd)    { invited.insert(fd); }
void Channel::removeInvited(int fd) { invited.erase(fd); }
