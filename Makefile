NAME = ircserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = srcs/main.cpp \
	srcs/Command.cpp \
	srcs/Channel.cpp \
	srcs/Client.cpp \
	srcs/Server.cpp \
	srcs/Replies.cpp \
	srcs/commands/Pass.cpp \
	srcs/commands/Nick.cpp \
	srcs/commands/User.cpp \
	srcs/commands/Quit.cpp \
	srcs/commands/Join.cpp \
	srcs/commands/Privmsg.cpp \
	srcs/commands/Notice.cpp \
	srcs/commands/Part.cpp \
	srcs/commands/Topic.cpp \
	srcs/commands/Invite.cpp \
	srcs/commands/Kick.cpp \
	srcs/commands/Mode.cpp

INCD = includes/

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCD) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
