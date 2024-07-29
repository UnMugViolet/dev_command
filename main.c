#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "program/program.h"
#include "settings/settings.h"

struct web_browser *selected_browser = NULL;

void save_selected_browser(struct web_browser *browser)
{
	FILE *file = fopen("selected_browser.dat", "wb");
	if (file == NULL)
	{
		perror("Failed to open file for writing");
		exit(EXIT_FAILURE);
	}
	fwrite(browser, sizeof(struct web_browser), 1, file);
	fclose(file);
}

struct web_browser *load_selected_browser()
{
	FILE *file = fopen("selected_browser.dat", "rb");
	if (file == NULL)
	{
		return NULL;
	}
	struct web_browser *browser = malloc(sizeof(struct web_browser));
	if (browser == NULL)
	{
		perror("Failed to allocate memory for browser");
		fclose(file);
		return NULL;
	}
	fread(browser, sizeof(struct web_browser), 1, file);
	fclose(file);
	return browser;
}

void init()
{
	selected_browser = load_selected_browser();
	if (selected_browser == NULL)
	{
		initialize_browsers();
		list_browsers();
		selected_browser = choose_browser();
		if (selected_browser != NULL)
		{
			save_selected_browser(selected_browser);
			printf("You have selected: %s\n", selected_browser->name);
		}
		else
		{
			fprintf(stderr, "No browser selected. Exiting...\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		printf("Previously selected browser: %s\n", selected_browser->name);
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
	if (selected_browser == NULL)
	{
		init();
		return (0);
	}

	if (argc > 3)
	{
		help();
	}
	if (argc == 2)
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