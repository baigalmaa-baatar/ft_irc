NAME		=	ircserv

SDIR		=	srcs

SRCS		=	main.cpp \
				initialize_data.cpp error_exit.cpp find_client.cpp \
				utils.cpp delete_client.cpp \
				classes/User.cpp classes/Registration.cpp classes/Buffer.cpp \
				classes/Client.cpp classes/Channel.cpp \
				parsing/parsing.cpp parsing/create_msg.cpp \
				parsing/parse_command.cpp parsing/parse_params.cpp parsing/match_pattern.cpp\
				initialize_commands.cpp commands/CAP.cpp commands/PASS.cpp \
				commands/NICK.cpp commands/USER.cpp commands/PING.cpp commands/PONG.cpp \
				commands/QUIT.cpp commands/ERROR.cpp commands/JOIN.cpp commands/MODE.cpp \
				commands/PRIVMSG.cpp commands/PART.cpp commands/KICK.cpp commands/INVITE.cpp \
				commands/TOPIC.cpp commands/NAMES.cpp commands/OPER.cpp commands/DEBUG.cpp\
				commands/MOTD.cpp commands/KILL.cpp \
				replies/send_replies.cpp replies/create_rpls.cpp \
				replies/RPL_replies.cpp replies/ERR_replies.cpp

IDIR		=	incs

ODIR		=	objs

OBJS		=	$(addprefix $(ODIR)/,$(SRCS:.cpp=.o))

RM			=	rm -rf

CXX			=	c++

# CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

$(ODIR)/%.o	:	$(SDIR)/%.cpp $(INCS)
			@mkdir -p $(@D)
			$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME)		:	$(OBJS)
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all			:	$(NAME)

clean		:
			$(RM) $(ODIR)

fclean		:	clean
			$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re
