NAME = ircserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = srcs/main.cpp \
	srcs/Command.cpp \
	srcs/Channel.cpp \
	srcs/Client.cpp \
	srcs/Server.cpp \
	srcs/Pass.cpp \
	srcs/Nick.cpp \
	srcs/User.cpp \
	srcs/Quit.cpp \
	srcs/Join.cpp \
	srcs/Replies.cpp \
	srcs/Privmsg.cpp \
	srcs/Notice.cpp \
	srcs/Part.cpp \
	srcs/Topic.cpp \
	srcs/Invite.cpp \
	srcs/Kick.cpp \
	srcs/Mode.cpp

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
