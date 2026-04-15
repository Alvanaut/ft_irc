#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <set>
# include <string>

class Channel
{
private:
	std::string		name;
	std::set<int>	members;
	std::set<int>	operators;
	std::set<int>	invited;
	std::string		topic;
	std::string		key;
	bool			invite_only;
	bool			topic_restricted;
	int				user_limit;

public:
	Channel();
	explicit Channel(const std::string& name);
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
	~Channel();

	const std::string&		getName() const;
	const std::set<int>&	getMembers() const;
	const std::set<int>&	getOperators() const;
	const std::string&		getTopic() const;
	const std::string&		getKey() const;
	bool					isInviteOnly() const;
	bool					isTopicRestricted() const;
	int						getUserLimit() const;

	void	setTopic(const std::string& t);
	void	setKey(const std::string& k);
	void	setInviteOnly(bool value);
	void	setTopicRestricted(bool value);
	void	setUserLimit(int limit);

	bool	hasMember(int fd) const;
	bool	isOperator(int fd) const;
	bool	isInvited(int fd) const;
	bool	empty() const;

	void	addMember(int fd);
	void	removeMember(int fd);
	void	addOperator(int fd);
	void	removeOperator(int fd);
	void	addInvited(int fd);
	void	removeInvited(int fd);
};

#endif // CHANNEL_HPP
