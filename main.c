#include <string.h>
#include "program/program.h"
#include "settings/settings.h"

int	check_argv(char *argv, char *str)
{
	if (strcmp(argv, str) == 0)
	{
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc > 3)
	{
		help();
	}
	if (argc == 2)
	{
		if (check_argv(argv[1], "settings"))
		{
			settings_menu();
		} else if (check_argv(argv[1], "help"))
		{
			help();
		} else if (check_argv(argv[1], "list"))
		{
			list_workspaces();
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
	return (0);
}