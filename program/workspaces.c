#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../settings/settings.h"
#include "../program/workspaces.h"
#include "../utils/utils.h"

void start_workspace(char *project)
{
	struct web_browser *selected_browser = load_selected_browser();
	if (selected_browser == NULL)
	{
		fprintf(stderr, "No browser selected. Please run the init process first.\n");
		exit(EXIT_FAILURE);
	}

	int workspace_id = find_workspace(project);

	if (workspace_id == -1)
	{
		fprintf(stderr, "Workspace %s not found\n", project);
		exit(EXIT_FAILURE);
	}

	// Change the current directory to the workspace path
	if (chdir(workspaces[workspace_id].path) == -1)
	{
		perror("Failed to change directory to workspace path");
		exit(EXIT_FAILURE);
	}

	// Open the code editor
	if (workspaces[workspace_id].editor)
	{
		if (workspaces[workspace_id].editor->command)
		{
			system(workspaces[workspace_id].editor->command);
		}
		perror("Failed to open code editor");
		exit(EXIT_FAILURE);
	}

	// Check if the project needs sudoer before starting
	if (workspaces[workspace_id].need_sudoer)
	{
		if (workspaces[workspace_id].need_sudoer)
		{
			system("sudo -v");
		}
	}

	// Start the project
	if (workspaces[workspace_id].start_command)
	{
		for (int i = 0; workspaces[workspace_id].start_command[i]; i++)
		{
			system(workspaces[workspace_id].start_command[i]);
			system(" & ");
		}
	}

	// Wait for the project to start
	sleep(4);

	// Open the browser
	if (workspaces[workspace_id].urls)
	{
		for (int i = 0; workspaces[workspace_id].urls[i]; i++)
		{
			char command[120];
			sprintf(command, "%s %s", selected_browser->command, workspaces[workspace_id].urls[i]);
			system(command);
		}
	}
}

void stop_workspace(char *project)
{

	int workspace_id = find_workspace(project);

	if (workspace_id == -1)
	{
		fprintf(stderr, "Workspace %s not found\n", project);
		exit(EXIT_FAILURE);
	}

	printf("Stopping project %s\n", project);
	// Stop the project
	if (workspaces[workspace_id].stop_command)
	{
		for (int i = 0; workspaces[workspace_id].stop_command[i]; i++)
		{
			system(workspaces[workspace_id].stop_command[i]);
		}
	}
	fprintf(stderr, "Failed to stop project %s\n", project);
}
