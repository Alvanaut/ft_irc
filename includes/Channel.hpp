#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <set>
# include <string>

class Channel
{
private:
	std::string		name;
	std::set<int>	members;

public:
	Channel();
	explicit Channel(const std::string& name);
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
	~Channel();

	const std::string&	getName() const;
	const std::set<int>&	getMembers() const;

	bool	hasMember(int fd) const;
	bool	empty() const;
	void	addMember(int fd);
	void	removeMember(int fd);
};

#endif // CHANNEL_HPP
