#include <stdio.h>
#include <stdlib.h>
#include "settings.h"

void	add_workspace()
{}

void	remove_workspace()
{}

void  settings_menu()
{
	printf("SETTINGS MENU\n");
	unsigned int choice;
	printf("1. Add workspace\n");
	printf("2. Remove workspace\n");
	printf("3. Change browser\n");
	printf("Enter the number of the option you want to use: ");

	scanf("%d", &choice);
	switch (choice)
	{
		case 1:
			add_workspace();
			break;
		case 2:
			remove_workspace();
			break;
		case 3:
			initialize_browsers();
			list_browsers();
			struct web_browser *selected_browser = choose_browser();
			if (selected_browser != NULL)
			{
				save_selected_browser(selected_browser);
				printf("You have selected: %s\n", selected_browser->name);
			}
			break;
		default:
			printf("Invalid choice\n");
			break;
	}
}

void	help(void)
{
	printf("=====================Workspace creator=====================\n");
	printf("Launch or stop workspace projects on the machine.\n");
	printf("Add/Remove project: workspace [add | remove] [project_name].\n");
	printf("Launch project: workspace [project_name] [up | down].\n");
	printf("Settings: workspace settings -> Allow to change the default browser and code editor.\n");
	printf("List: workspace list -> List all available projects\n");
	printf("Any issue? Contact me at paul@rocketegg.systems\nMade by Paul Jaguin.\n");
}

void	usage(void)
{
	printf("Command not found.\n");
	printf("Add/Remove project: workspace [add | remove] [project_name].\n");
	printf("Launch project: workspace [project_name] [up | down].\n");
	printf("Settings: workspace settings\n");
	printf("List projects: workspace list\n");
}