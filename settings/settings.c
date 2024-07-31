#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "../program/workspaces.h"
#include "../utils/utils.h"

void save_workspace(struct workspaces *workspace)
{
	FILE *file = fopen("workspaces.dat", "ab");
	if (file == NULL)
	{
		perror("Failed to open file for writing");
		exit(EXIT_FAILURE);
	}
	if (fwrite(workspace, sizeof(struct workspaces), 1, file) != 1)
	{
		perror("Failed to write workspace data to file");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	printf("Workspace saved\n");
	printf("Workspace id: %i\n", workspace->id);
	fclose(file);
}

void add_workspace(char *workspace_name)
{
	printf("Adding workspace %s\n", workspace_name);

	// Allocate memory for the new workspace
	struct workspaces *new_workspace = (struct workspaces *)malloc(sizeof(struct workspaces));
	if (new_workspace == NULL)
	{
		perror("Failed to allocate memory for new workspace");
		exit(EXIT_FAILURE);
	}

	// Get the number of workspaces
	int num_workspaces = get_workspace_id();

	// Set the workspace ID
	new_workspace->id = num_workspaces;

	// Copy the workspace name
	strncpy(new_workspace->name, workspace_name, sizeof(new_workspace->name) - 1);
	new_workspace->name[sizeof(new_workspace->name) - 1] = '\0';

	// Set default values for other fields
	strcpy(new_workspace->path, "/home/paul/");
	new_workspace->editor = NULL;
	new_workspace->urls = NULL;
	new_workspace->start_command = NULL;
	new_workspace->stop_command = NULL;
	new_workspace->need_sudoer = 0;

	// Save the workspace
	save_workspace(new_workspace);

	// Reallocate memory for the workspaces array
	workspaces = (struct workspaces *)realloc(workspaces, num_workspaces * sizeof(struct workspaces));
	if (workspaces == NULL)
	{
		perror("Failed to reallocate memory for workspaces array");
		exit(EXIT_FAILURE);
	}

	// Add the new workspace to the array
	workspaces[num_workspaces -1] = *new_workspace;

	// Free the allocated memory for the new workspace
	free(new_workspace);

	printf("Workspace %s added successfully\n", workspace_name);
}

void remove_workspace(char *workspace_name)
{
	// TODO: Implement remove_workspace function
	// Implement validation 
	int valid = 0;

	if (find_workspace(workspace_name))
	{
		printf("Are you sure you want to remove %s? [0 no : 1 yes]\n", workspace_name);
		scanf("%d", &valid);
		if (valid)
		{
			// Remove the workspace
			// TODO: Implement remove_workspace function
			printf("Workspace %s removed successfully\n", workspace_name);
		}
		else
		{
			printf("Operation cancelled\n");
			exit(EXIT_SUCCESS);
		}
	} else {
		fprintf(stderr, "Workspace %s not found\n", workspace_name);
	}
}

void modify_workspace()
{
}

void settings_menu()
{
	printf("SETTINGS MENU\n");
	unsigned int choice;
	printf("1. Modify workspace\n");
	printf("2. Change browser\n");
	printf("Enter the number of the option you want to use: ");

	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		modify_workspace();
		break;
	case 2:
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

void help(void)
{
	printf("=====================Workspace creator=====================\n");
	printf("Launch or stop workspace projects on the machine.\n");
	printf("Add/Remove project: workspace [add | remove] [project_name].\n");
	printf("Launch project: workspace [project_name] [up | down].\n");
	printf("Settings: workspace settings -> Allow to change the default browser and code editor.\n");
	printf("List: workspace list -> List all available projects\n");
	printf("Any issue? Contact me at paul@rocketegg.systems\nMade by Paul Jaguin.\n");
}

void usage(void)
{
	printf("Command not found.\n");
	printf("Add/Remove project: workspace [add | remove] [project_name].\n");
	printf("Launch project: workspace [project_name] [up | down].\n");
	printf("Settings: workspace settings\n");
	printf("List projects: workspace list\n");
}