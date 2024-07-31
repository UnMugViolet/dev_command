COMPILE = cc
CFLAGS = -Wextra -Wall -Werror
DEBUG = -g3
NAME = workspace
SRC = main.c program/workspaces.c settings/settings.c settings/web_browsers.c settings/code_editors.c utils/str_split.c utils/tools.c
DEPS = $(SRC:.c=.o)

$(NAME): $(DEPS)
	$(COMPILE) $(CFLAGS) -o $(NAME) $(DEPS)

%.o: %.c
	$(COMPILE) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(DEPS) $(NAME)

debug: $(DEPS)
	$(COMPILE) $(CFLAGS) $(DEBUG) -o $(NAME) $(DEPS)

re: clean $(NAME)

valgrind: debug
	valgrind --leak-check=full ./$(NAME) 2> valgrind.log