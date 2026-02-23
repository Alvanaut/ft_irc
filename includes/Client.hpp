#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include <iostream>
#include <cstring>


class Client {
    private:
        int _fd;
        std::string _buffer;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        bool        _registered;
        bool        _authenticated;

    public:
        Client();
        ~Client();
        Client(int fd);

        void        setBuffer(const std::string& buffer);
        void        setUsername(const std::string& username);
        void        setNickname(const std::string& nickname);
        void        setRealname(const std::string& realname);
        void        setRegistered(bool registered);
        void        setAuthenticated(bool authenticated);

        int         getFd() const;
        std::string getBuffer() const;
        std::string getUsername() const;
        std::string getNickname() const;
        std::string getRealname() const;
        bool        getRegistered() const;
        bool        getAuthenticated() const;

        void        appendToBuffer(const std::string& data);
        std::string extractMessage();

};

#endif