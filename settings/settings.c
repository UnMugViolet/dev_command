#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "../program/workspaces.h"
#include "../utils/utils.h"

void save_workspace(struct workspaces *ws, const char *filename)
{
	FILE *file = fopen(filename, "ab");
	if (file == NULL)
	{
		perror("Failed to open file");
		return;
	}

	fwrite(ws, sizeof(struct workspaces), 1, file);

	if (ws->editor != NULL)
	{
		fwrite(ws->editor, sizeof(struct code_editor), 1, file);
	}

	// Save URLs
	if (ws->urls != NULL)
	{
		for (int i = 0; ws->urls[i] != NULL; i++)
		{
			size_t len = strlen(ws->urls[i]) + 1;
			fwrite(&len, sizeof(size_t), 1, file);
			fwrite(ws->urls[i], sizeof(char), len, file);
		}
		size_t end = 0;
		fwrite(&end, sizeof(size_t), 1, file); // Null terminator
	}

	// Save start commands
	if (ws->start_command != NULL)
	{
		for (int i = 0; ws->start_command[i] != NULL; i++)
		{
			size_t len = strlen(ws->start_command[i]) + 1;
			fwrite(&len, sizeof(size_t), 1, file);
			fwrite(ws->start_command[i], sizeof(char), len, file);
		}
		size_t end = 0;
		fwrite(&end, sizeof(size_t), 1, file);
	}

	// Save stop commands
	if (ws->stop_command != NULL)
	{
		for (int i = 0; ws->stop_command[i] != NULL; i++)
		{
			size_t len = strlen(ws->stop_command[i]) + 1;
			fwrite(&len, sizeof(size_t), 1, file);
			fwrite(ws->stop_command[i], sizeof(char), len, file);
		}
		size_t end = 0;
		fwrite(&end, sizeof(size_t), 1, file);
	}

	fclose(file);
}

void add_workspace_form(struct workspaces *ws)
{
	char input[256];

	system("clear");
	printf("ADD WORKSPACE\n");
	printf("Enter the workspace path: \n");
	get_user_input(input, sizeof(input));
	process_input(input, ws->path, 0);

	system("clear");
	printf("Choose a code editor: \n");
	ws->editor = choose_code_editor();
	if (ws->editor == NULL)
	{
		printf("No code editor selected\n");
	}

	// Clear the input buffer
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
	{
	}

	system("clear");
	printf("Enter the URLs separated by a comma [,]: \n");
	get_user_input(input, sizeof(input));
	process_input(input, &(ws->urls), 1);
}

int summarize_form_and_confirm(struct workspaces *ws)
{
	int confirm;

	system("clear");
	printf("Workspace id: %i\n", ws->id);
	printf("Workspace name: %s\n", ws->name);
	printf("Workspace path: %s\n", ws->path);
	printf("Code editor: ");
	if (ws->editor == NULL)
	{
		printf("None\n");
	}
	else
	{
		printf("%s\n", ws->editor->name);
	}
	printf("URLs: ");
	if (ws->urls == NULL)
	{
		printf("None\n");
	}
	else
	{
		for (char **url = ws->urls; *url; url++)
		{
			printf("%s ", *url);
		}
		printf("\n");
	}
	printf("Start commands: ");
	if (ws->start_command == NULL)
	{
		printf("None\n");
	}
	else
	{
		for (int i = 0; ws->start_command[i]; i++)
		{
			printf("%s ", ws->start_command[i]);
		}
		printf("\n");
	}
	printf("Stop commands: ");
	if (ws->stop_command == NULL)
	{
		printf("None\n");
	}
	else
	{
		for (int i = 0; ws->stop_command[i]; i++)
		{
			printf("%s ", ws->stop_command[i]);
		}
		printf("\n");
	}
	printf("Need sudoer: %d\n", ws->need_sudoer);

	printf("Do you want to save the workspace? [0 no : 1 yes]\n");
	scanf("%d", &confirm);

	return confirm;
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

	// Set the workspace ID
	new_workspace->id = get_workspace_id();

	// Copy the workspace name
	strncpy(new_workspace->name, workspace_name, sizeof(new_workspace->name) - 1);
	new_workspace->name[sizeof(new_workspace->name) - 1] = '\0';

	// Initialize the workspace path
	add_workspace_form(new_workspace);

	new_workspace->start_command = NULL;
	new_workspace->stop_command = NULL;
	new_workspace->need_sudoer = 0;

	int confirm = summarize_form_and_confirm(new_workspace);

	// Save the workspace if confirmed
	if (confirm)
	{
		save_workspace(new_workspace, "workspaces.dat");
		printf("Workspace %s added successfully\n", workspace_name);
	}
	else
		printf("Workspace not saved\n");

	// Reallocate memory for the workspaces array
	workspaces = (struct workspaces *)realloc(workspaces, (num_workspaces + 1) * sizeof(struct workspaces));
	if (workspaces == NULL)
	{
		perror("Failed to reallocate memory for workspaces array");
		exit(EXIT_FAILURE);
	}

	// Add the new workspace to the array
	workspaces[num_workspaces - 1] = *new_workspace;

	// Free the allocated memory for the new workspace
	free(new_workspace);
}

void remove_workspace(char *workspace_name)
{
	int valid = 0;

	if (find_workspace(workspace_name) != -1)
	{
		printf("Are you sure you want to remove %s? [0 no : 1 yes]\n", workspace_name);
		scanf("%d", &valid);
		if (valid)
		{
			FILE *file = fopen("workspaces.dat", "rb");
			if (file == NULL)
			{
				fprintf(stderr, "Failed to open file. workspace add [name] to add a project.\n");
				return;
			}

			FILE *temp_file = fopen("temp.dat", "wb");
			if (temp_file == NULL)
			{
				fclose(file);
				fprintf(stderr, "Failed to open temp.dat\n");
				return;
			}

			struct workspaces workspace;
			int found = 0;

			while (fread(&workspace, sizeof(struct workspaces), 1, file))
			{
				if (strcmp(workspace.name, workspace_name) != 0)
					fwrite(&workspace, sizeof(struct workspaces), 1, temp_file);
				else
					found = 1;
			}

			fclose(file);
			fclose(temp_file);

			if (found)
			{
				remove("workspaces.dat");
				rename("temp.dat", "workspaces.dat");

				// Reallocate memory for the workspaces array
				workspaces = (struct workspaces *)realloc(workspaces, (num_workspaces - 1) * sizeof(struct workspaces));
				if (workspaces == NULL)
				{
					perror("Failed to reallocate memory for workspaces array");
					exit(EXIT_FAILURE);
				}

				printf("Workspace %s removed successfully\n", workspace_name);
			}
			else
			{
				remove("temp.dat");
				fprintf(stderr, "Workspace %s not found in the file\n", workspace_name);
			}
		}
		else
		{
			printf("Operation cancelled\n");
			exit(EXIT_SUCCESS);
		}
	}
	else
		fprintf(stderr, "Workspace %s not found\n", workspace_name);
}

void settings_menu()
{
	printf("SETTINGS MENU\n");
	unsigned int choice;
	printf("1. Change browser\n");
	printf("Enter the number of the option you want to use: ");

	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
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