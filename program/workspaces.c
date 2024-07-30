#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../settings/settings.h"
#include "../workspaces.h"

int count_workspaces()
{
	FILE *file = fopen("workspaces.dat", "rb");
	if (file == NULL)
	{
		// If the file does not exist, return 0 workspaces
		if (errno == ENOENT)
		{
			return 0;
		}
		else
		{
			perror("Failed to open file for reading");
			exit(EXIT_FAILURE);
		}
	}
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fclose(file);
	return file_size / sizeof(struct workspaces);
}

void list_workspaces()
{
	FILE *file = fopen("workspaces.dat", "rb");
	if (file == NULL)
	{
		perror("Failed to open file for reading");
		exit(EXIT_FAILURE);
	}
	struct workspaces workspace;
	while (fread(&workspace, sizeof(struct workspaces), 1, file))
	{
		printf("Workspace id: %i\n", workspace.id);
		printf("Workspace name: %s\n", workspace.name);
		printf("\tWorkspace path: %s\n", workspace.path);
		printf("\tURLs: ");
		for (int i = 0; workspace.urls[i]; i++)
		{
			printf("\t\t%s ", workspace.urls[i]);
		}
		printf("\n");
		printf("\tStart commands: ");
		for (int i = 0; workspace.start_command[i]; i++)
		{
			printf("\t\t%s ", workspace.start_command[i]);
		}
		printf("\n");
		printf("Stop commands: ");
		for (int i = 0; workspace.stop_command[i]; i++)
		{
			printf("%s ", workspace.stop_command[i]);
		}
		printf("\n");
		printf("Need sudoer: %d\n", workspace.need_sudoer);
	}
	fclose(file);
}

int find_workspace(char *project)
{
	FILE *file = fopen("workspaces.dat", "rb");
	if (file == NULL)
	{
		perror("Failed to open file for reading");
		exit(EXIT_FAILURE);
	}
	struct workspaces workspace;
	while (fread(&workspace, sizeof(struct workspaces), 1, file))
	{
		if (strcmp(workspace.name, project) == 0)
		{
			fclose(file);
			return workspace.id;
		}
	}
	fclose(file);
	fprintf(stderr, "Workspace %s not found\n", project);
	return -1;
}

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
			system("&");
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
