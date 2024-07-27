COMPILE = cc
CFLAGS = -Wextra -Wall -Werror
NAME = workspace
SRC = main.c program/workspaces.c settings/settings.c settings/web_browsers.c settings/code_editors.c
DEPS = $(SRC:.c=.o)

$(NAME): $(DEPS)
	$(COMPILE) $(CFLAGS) -o $(NAME) $(DEPS)

%.o: %.c
	$(COMPILE) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(DEPS) $(NAME)
