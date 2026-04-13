#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <set>
# include <string>

class Client
{
private:
	int					fd;
	std::string			input_buffer;
	bool				password_accepted;
	bool				has_nickname;
	bool				has_username;
	bool				registered;
	std::string			nickname;
	std::string			old_nickname;
	std::string			username;
	std::string			real_name;
	std::set<std::string>	channels_joined;

public:
	Client();
	explicit Client(int fd);
	Client(const Client& other);
	Client& operator=(const Client& other);
	~Client();

	int					getFd() const;
	const std::string&	getInputBuffer() const;
	bool				isPasswordAccepted() const;
	bool				hasNickname() const;
	bool				hasUsername() const;
	bool				isRegistered() const;
	const std::string&	getNickname() const;
	const std::string&	getOldNickname() const;
	const std::string&	getUsername() const;
	const std::string&	getRealName() const;
	const std::set<std::string>& getChannelsJoined() const;

	void	setFd(int new_fd);
	void	appendToInputBuffer(const std::string& data);
	void	clearInputBuffer();
	void	eraseFromInputBuffer(size_t count);
	void	setPasswordAccepted(bool accepted);
	void	setNickname(const std::string& new_nickname);
	void	setUsername(const std::string& new_username);
	void	setRealName(const std::string& new_real_name);
	void	setRegistered(bool value);
	void	joinChannel(const std::string& channel);
	void	leaveChannel(const std::string& channel);
	bool	isInChannel(const std::string& channel) const;
	bool	registrationComplete() const;
};

#endif // CLIENT_HPP
