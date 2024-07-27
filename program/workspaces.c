#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct workspaces
{
	int id;
	char name [50];
	char path [120];
	char **urls;
	char *launch_command;
	char *stop_command;
	unsigned int need_sudoer;
} *workspaces;

int find_workspace(char *project)
{
	int i = 0;

	while (workspaces[i].id)
	{
		if (strcmp(workspaces[i].name, project) == 0)
		{
			return workspaces[i].id;
		}
		i++;
	}
	printf("Project not found\n");
	return -1;
}

void start_project(char *project)
{

	int workspace_id = find_workspace(project);

	if (workspace_id != -1)
	{
		int i = 0;
		printf("Starting project %s\n", project);
		while (workspaces[i].id == workspace_id)
		{
			if (workspaces[i].need_sudoer)
			{
				printf("Please enter your password in order to start the project\n");
				system("sudo -v");
				system(workspaces[i].launch_command);
			}
			if (workspaces[i].launch_command)
			{
				system(workspaces[i].launch_command);
			}
			i++;
		}
	}
}

void stop_project(char *project)
{

	int workspace_id = find_workspace(project);

	if (workspace_id != -1)
	{
		int i = 0;
		printf("Stopping project %s\n", project);
		while (workspaces[i].id == workspace_id)
		{
			if (workspaces[i].stop_command)
			{
				system(workspaces[i].stop_command);
			}
			i++;
		}
	}
}
