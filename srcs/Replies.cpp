#include "../includes/Replies.hpp"

static const std::string SRV = ":ircserv ";

namespace RPL
{
	std::string welcome(const std::string& nick, const std::string& user)
	{
		return SRV + "001 " + nick + " :Welcome to the IRC Network "
			+ nick + "!" + user + "@ircserv\r\n";
	}

	std::string yourHost(const std::string& nick)
	{
		return SRV + "002 " + nick + " :Your host is ircserv, running version 1.0\r\n";
	}

	std::string created(const std::string& nick)
	{
		return SRV + "003 " + nick + " :This server was created long ago\r\n";
	}

	std::string myInfo(const std::string& nick)
	{
		return SRV + "004 " + nick + " ircserv 1.0 io itkol\r\n";
	}

	std::string noTopic(const std::string& nick, const std::string& chan)
	{
		return SRV + "331 " + nick + " " + chan + " :No topic is set\r\n";
	}

	std::string topic(const std::string& nick, const std::string& chan,
	                  const std::string& t)
	{
		return SRV + "332 " + nick + " " + chan + " :" + t + "\r\n";
	}

	std::string inviting(const std::string& nick, const std::string& target,
	                     const std::string& chan)
	{
		return SRV + "341 " + nick + " " + target + " " + chan + "\r\n";
	}

	std::string namReply(const std::string& nick, const std::string& chan,
	                     const std::string& names)
	{
		return SRV + "353 " + nick + " = " + chan + " :" + names + "\r\n";
	}

	std::string endOfNames(const std::string& nick, const std::string& chan)
	{
		return SRV + "366 " + nick + " " + chan + " :End of /NAMES list\r\n";
	}
}

namespace ERR
{
	std::string noSuchNick(const std::string& nick, const std::string& target)
	{
		return SRV + "401 " + nick + " " + target + " :No such nick/channel\r\n";
	}

	std::string noSuchChannel(const std::string& nick, const std::string& chan)
	{
		return SRV + "403 " + nick + " " + chan + " :No such channel\r\n";
	}

	std::string cannotSendToChan(const std::string& nick, const std::string& chan)
	{
		return SRV + "404 " + nick + " " + chan + " :Cannot send to channel\r\n";
	}

	std::string noRecipient(const std::string& nick, const std::string& cmd)
	{
		return SRV + "411 " + nick + " :No recipient given (" + cmd + ")\r\n";
	}

	std::string noTextToSend(const std::string& nick)
	{
		return SRV + "412 " + nick + " :No text to send\r\n";
	}

	std::string noNicknameGiven(const std::string& nick)
	{
		return SRV + "431 " + nick + " :No nickname given\r\n";
	}

	std::string erroneusNickname(const std::string& nick, const std::string& bad)
	{
		return SRV + "432 " + nick + " " + bad + " :Erroneous nickname\r\n";
	}

	std::string nicknameInUse(const std::string& nick, const std::string& bad)
	{
		return SRV + "433 " + nick + " " + bad + " :Nickname is already in use\r\n";
	}

	std::string userNotInChannel(const std::string& nick, const std::string& user,
	                             const std::string& chan)
	{
		return SRV + "441 " + nick + " " + user + " " + chan
			+ " :They aren't on that channel\r\n";
	}

	std::string notOnChannel(const std::string& nick, const std::string& chan)
	{
		return SRV + "442 " + nick + " " + chan + " :You're not on that channel\r\n";
	}

	std::string userOnChannel(const std::string& nick, const std::string& target,
	                          const std::string& chan)
	{
		return SRV + "443 " + nick + " " + target + " " + chan
			+ " :is already on channel\r\n";
	}

	std::string needMoreParams(const std::string& nick, const std::string& cmd)
	{
		return SRV + "461 " + nick + " " + cmd + " :Not enough parameters\r\n";
	}

	std::string alreadyRegistered(const std::string& nick)
	{
		return SRV + "462 " + nick + " :You may not reregister\r\n";
	}

	std::string passwdMismatch(const std::string& nick)
	{
		return SRV + "464 " + nick + " :Password incorrect\r\n";
	}

	std::string channelIsFull(const std::string& nick, const std::string& chan)
	{
		return SRV + "471 " + nick + " " + chan + " :Cannot join channel (+l)\r\n";
	}

	std::string unknownMode(const std::string& nick, char mode)
	{
		return SRV + "472 " + nick + " " + std::string(1, mode)
			+ " :is unknown mode char to me\r\n";
	}

	std::string inviteOnlyChan(const std::string& nick, const std::string& chan)
	{
		return SRV + "473 " + nick + " " + chan + " :Cannot join channel (+i)\r\n";
	}

	std::string badChannelKey(const std::string& nick, const std::string& chan)
	{
		return SRV + "475 " + nick + " " + chan + " :Cannot join channel (+k)\r\n";
	}

	std::string badChanMask(const std::string& nick, const std::string& chan)
	{
		return SRV + "476 " + nick + " " + chan + " :Bad Channel Mask\r\n";
	}

	std::string chanopPrivsNeeded(const std::string& nick, const std::string& chan)
	{
		return SRV + "482 " + nick + " " + chan + " :You're not channel operator\r\n";
	}
}
