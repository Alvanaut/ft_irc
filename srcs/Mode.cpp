#include "../includes/Mode.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Replies.hpp"

#include <cstdlib>

Mode::Mode(const Message& msg) : Command(msg) {}

// Ajoute le signe courant dans la string de réponse si nécessaire
static void appendSign(std::string& modeStr, char& lastSign, char sign)
{
	if (lastSign != sign)
	{
		modeStr += sign;
		lastSign = sign;
	}
}

int Mode::execute(Client& client, Server& server)
{
	const std::string& nick = client.getNickname();
	const std::string& user = client.getUsername();

	if (_msg.params.empty())
	{
		server.sendToClient(client.getFd(), ERR::needMoreParams(nick, "MODE"));
		return (0);
	}

	const std::string& chanName = _msg.params[0];
	Channel* ch = server.getChannel(chanName);

	if (!ch)
	{
		server.sendToClient(client.getFd(), ERR::noSuchChannel(nick, chanName));
		return (0);
	}
	if (!ch->hasMember(client.getFd()))
	{
		server.sendToClient(client.getFd(), ERR::notOnChannel(nick, chanName));
		return (0);
	}

	// MODE sans modestring → afficher les modes actifs (comportement basique)
	if (_msg.params.size() < 2)
		return (0);

	if (!ch->isOperator(client.getFd()))
	{
		server.sendToClient(client.getFd(), ERR::chanopPrivsNeeded(nick, chanName));
		return (0);
	}

	const std::string& modeString = _msg.params[1];
	char               sign       = '+';
	size_t             paramIdx   = 2;    // index dans _msg.params pour les args de modes

	std::string appliedModes;             // ex: "+it-k"
	std::string appliedParams;            // ex: "password"
	char        lastSign = 0;

	for (size_t i = 0; i < modeString.size(); ++i)
	{
		char c = modeString[i];

		if (c == '+' || c == '-')
		{
			sign = c;
			continue ;
		}

		switch (c)
		{
			case 'i':
				ch->setInviteOnly(sign == '+');
				appendSign(appliedModes, lastSign, sign);
				appliedModes += 'i';
				break ;

			case 't':
				ch->setTopicRestricted(sign == '+');
				appendSign(appliedModes, lastSign, sign);
				appliedModes += 't';
				break ;

			case 'k':
				if (sign == '+')
				{
					if (paramIdx >= _msg.params.size())
					{
						server.sendToClient(client.getFd(),
							ERR::needMoreParams(nick, "MODE"));
						break ;
					}
					const std::string& key = _msg.params[paramIdx++];
					ch->setKey(key);
					appendSign(appliedModes, lastSign, sign);
					appliedModes += 'k';
					appliedParams += (appliedParams.empty() ? "" : " ") + key;
				}
				else
				{
					ch->setKey("");
					appendSign(appliedModes, lastSign, sign);
					appliedModes += 'k';
				}
				break ;

			case 'o':
			{
				if (paramIdx >= _msg.params.size())
				{
					server.sendToClient(client.getFd(),
						ERR::needMoreParams(nick, "MODE"));
					break ;
				}
				const std::string& targetNick = _msg.params[paramIdx++];
				const Client* target = server.getClientByNick(targetNick);
				if (!target || !ch->hasMember(target->getFd()))
				{
					server.sendToClient(client.getFd(),
						ERR::userNotInChannel(nick, targetNick, chanName));
					break ;
				}
				if (sign == '+')
					ch->addOperator(target->getFd());
				else
					ch->removeOperator(target->getFd());
				appendSign(appliedModes, lastSign, sign);
				appliedModes += 'o';
				appliedParams += (appliedParams.empty() ? "" : " ") + targetNick;
				break ;
			}

			case 'l':
				if (sign == '+')
				{
					if (paramIdx >= _msg.params.size())
					{
						server.sendToClient(client.getFd(),
							ERR::needMoreParams(nick, "MODE"));
						break ;
					}
					int limit = std::atoi(_msg.params[paramIdx++].c_str());
					if (limit > 0)
					{
						ch->setUserLimit(limit);
						appendSign(appliedModes, lastSign, sign);
						appliedModes += 'l';
						appliedParams += (appliedParams.empty() ? "" : " ")
							+ _msg.params[paramIdx - 1];
					}
				}
				else
				{
					ch->setUserLimit(0);
					appendSign(appliedModes, lastSign, sign);
					appliedModes += 'l';
				}
				break ;

			default:
				server.sendToClient(client.getFd(), ERR::unknownMode(nick, c));
				break ;
		}
	}

	if (!appliedModes.empty())
	{
		std::string broadcast = ":" + nick + "!" + user + "@ircserv MODE "
			+ chanName + " " + appliedModes;
		if (!appliedParams.empty())
			broadcast += " " + appliedParams;
		broadcast += "\r\n";
		server.broadcastToChannel(chanName, broadcast);
	}
	return (0);
}
