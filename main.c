#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "program/workspaces.h"
#include "settings/settings.h"

struct web_browser *selected_browser = NULL;
struct workspaces *workspaces = NULL;
int num_workspaces = 0;

void init()
{
	initialize_browsers();
	list_browsers();
	selected_browser = choose_browser();
	if (selected_browser != NULL)
	{
		save_selected_browser(selected_browser);
		printf("You have selected: %s\nInitialization finished...\n", selected_browser->name);
	}
	else
	{
		fprintf(stderr, "No browser selected. Exiting...\n");
		exit(EXIT_FAILURE);
	}
}

int check_argv(char *argv, char *str)
{
	if (strcmp(argv, str) == 0)
	{
		return (1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	selected_browser = load_selected_browser();
	num_workspaces = count_workspaces();

	if (selected_browser == NULL)
	{
		init();
	}
	if (argc > 3)
	{
		help();
	}
	else if (argc == 2)
	{
		if (check_argv(argv[1], "settings"))
		{
			settings_menu();
		}
		else if (check_argv(argv[1], "help"))
		{
			help();
		}
		else if (check_argv(argv[1], "list"))
		{
			list_workspaces();
		}
		else if (check_argv(argv[1], "init"))
		{
			init();
		}
		else
		{
			usage();
		}
	}
	else if (argc == 3)
	{
		if (check_argv(argv[1], "add"))
		{
			add_workspace(argv[2]);
		}
		else if (check_argv(argv[1], "remove"))
		{
			remove_workspace(argv[2]);
		}
		else
		{
			if (check_argv(argv[2], "up"))
			{
				start_workspace(argv[1]);
			}
			else if (check_argv(argv[2], "down"))
			{
				stop_workspace(argv[1]);
			}
			else
			{
				usage();
			}
		}
	}
	else
	{
		usage();
	}
	return (0);
}