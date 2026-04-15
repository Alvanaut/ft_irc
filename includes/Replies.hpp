#ifndef REPLIES_HPP
# define REPLIES_HPP

# include <string>

namespace RPL
{
	std::string welcome(const std::string& nick, const std::string& user);   // 001
	std::string yourHost(const std::string& nick);                            // 002
	std::string created(const std::string& nick);                             // 003
	std::string myInfo(const std::string& nick);                              // 004
	std::string noTopic(const std::string& nick, const std::string& chan);    // 331
	std::string topic(const std::string& nick, const std::string& chan,
	                  const std::string& t);                                  // 332
	std::string inviting(const std::string& nick, const std::string& target,
	                     const std::string& chan);                            // 341
	std::string namReply(const std::string& nick, const std::string& chan,
	                     const std::string& names);                           // 353
	std::string endOfNames(const std::string& nick, const std::string& chan); // 366
}

namespace ERR
{
	std::string noSuchNick(const std::string& nick,
	                       const std::string& target);                        // 401
	std::string noSuchChannel(const std::string& nick,
	                          const std::string& chan);                       // 403
	std::string cannotSendToChan(const std::string& nick,
	                             const std::string& chan);                    // 404
	std::string noRecipient(const std::string& nick,
	                        const std::string& cmd);                          // 411
	std::string noTextToSend(const std::string& nick);                        // 412
	std::string noNicknameGiven(const std::string& nick);                     // 431
	std::string erroneusNickname(const std::string& nick,
	                             const std::string& bad);                     // 432
	std::string nicknameInUse(const std::string& nick,
	                          const std::string& bad);                        // 433
	std::string userNotInChannel(const std::string& nick,
	                             const std::string& user,
	                             const std::string& chan);                    // 441
	std::string notOnChannel(const std::string& nick,
	                         const std::string& chan);                        // 442
	std::string userOnChannel(const std::string& nick,
	                          const std::string& target,
	                          const std::string& chan);                       // 443
	std::string needMoreParams(const std::string& nick,
	                           const std::string& cmd);                       // 461
	std::string alreadyRegistered(const std::string& nick);                   // 462
	std::string passwdMismatch(const std::string& nick);                      // 464
	std::string channelIsFull(const std::string& nick,
	                          const std::string& chan);                       // 471
	std::string unknownMode(const std::string& nick, char mode);              // 472
	std::string inviteOnlyChan(const std::string& nick,
	                           const std::string& chan);                      // 473
	std::string badChannelKey(const std::string& nick,
	                          const std::string& chan);                       // 475
	std::string badChanMask(const std::string& nick,
	                        const std::string& chan);                         // 476
	std::string chanopPrivsNeeded(const std::string& nick,
	                              const std::string& chan);                   // 482
}

#endif // REPLIES_HPP
