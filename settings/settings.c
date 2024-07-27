#include <stdio.h>

void  settings_menu()
{}

void	add_project()
{}

void	remove_project()
{}

void	usage(void)
{
	printf("=====================Workspace creator=====================\n");
	printf("Launch or stop dev projects on the machine.\n");
	printf("Add/Remove project: dev [add | remove] [project_name].\n");
	printf("Launch project: dev [project_name] [up | down].\n");
	printf("Settings: Allow to change the default browser and code editor.\n");
	printf("Any issue? Contact me at paul@rocketegg.systems.\nMade by Paul Jaguin.\n");
}