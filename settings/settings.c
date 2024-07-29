#include <stdio.h>

void  settings_menu()
{}

void	add_workspace()
{}

void	remove_workspace()
{}

void	help(void)
{
	printf("=====================Workspace creator=====================\n");
	printf("Launch or stop workspace projects on the machine.\n");
	printf("Add/Remove project: workspace [add | remove] [project_name].\n");
	printf("Launch project: workspace [project_name] [up | down].\n");
	printf("Settings: workspace settings -> Allow to change the default browser and code editor.\n");
	printf("List: workspace list -> List all available projects\n");
	printf("Any issue? Contact me at paul@rocketegg.systems.\nMade by Paul Jaguin.\n");
}

void	usage(void)
{
	printf("Command not found.\n");
	printf("Add/Remove project: workspace [add | remove] [project_name].\n");
	printf("Launch project: workspace [project_name] [up | down].\n");
	printf("Settings: workspace settings\n");
	printf("List projects: workspace list\n");
}