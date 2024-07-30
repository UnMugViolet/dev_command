#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "../program/program.h"
#include "../workspaces.h"

void clear_input_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

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
	fclose(file);
}

struct workspaces *workspaces = NULL;
int num_workspaces = 0;

void add_workspace(char *workspace_name)
{
	int validate = 0;

	// system("clear");
	printf("Adding workspace %s\n", workspace_name);
	struct workspaces workspace;

	workspace.id = count_workspaces() + 1;
	strcpy(workspace.name, workspace_name);

	printf("Enter workspace path: ");
	scanf("%s", workspace.path);
	clear_input_buffer();

	// system("clear");
	printf("Choose a code editor:\n");
	initialize_code_editor();
	struct code_editor *selected_editor = choose_code_editor();
	if (selected_editor != NULL)
	{
		workspace.editor = selected_editor;
		printf("You have selected: %s\n", selected_editor->name);
	}
	else
	{
		printf("No code editor selected\n");
		workspace.editor = NULL;
	}

	// system("clear");
	char urls_input[100];
	printf("Enter URLs (comma-separated): ");
	printf("Put comma [,] if no url\n");
	scanf("%s", urls_input);
	workspace.urls = urls_input;
	clear_input_buffer();

	// system("clear");
	char start_command_input[120];
	printf("Enter start commands (comma-separated): ");
	scanf("%s", start_command_input);
	workspace.start_command = start_command_input;
	clear_input_buffer();

	// system("clear");
	char stop_command_input[120];
	printf("Enter stop commands (comma-separated): ");
	printf("Put comma [,] if no stop command\n");
	scanf("%s", stop_command_input);
	workspace.stop_command = stop_command_input;
	clear_input_buffer();

	// system("clear");
	printf("Do you need sudoer rights to run this workspace? (1 for yes, 0 for no): ");
	scanf("%d", &workspace.need_sudoer);
	clear_input_buffer();
	if (workspace.need_sudoer != 0 && workspace.need_sudoer != 1)
	{
		printf("Invalid choice\n");
		return;
	}

	// Print the entire form
	// system("clear");
	printf("Workspace name: %s\n", workspace_name);
	printf("Workspace path: %s\n", workspace.path);
	printf("URLs: ");
	for (int i = 0; workspace.urls[i] != NULL; i++)
	{
		printf("%s ", workspace.urls[i]);
	}
	printf("\n");
	printf("Start commands: ");
	for (int i = 0; workspace.start_command[i] != NULL; i++)
	{
		printf("%s ", workspace.start_command[i]);
	}
	printf("\n");
	printf("Stop commands: ");
	for (int i = 0; workspace.stop_command[i] != NULL; i++)
	{
		printf("%s ", workspace.stop_command[i]);
	}
	printf("\n");
	printf("Need sudoer rights: %d\n", workspace.need_sudoer);

	printf("Is this informations correct? (1 for yes, 0 for no): ");
	scanf("%d", &validate);
	clear_input_buffer();
	if (validate != 1)
	{
		printf("Workspace not saved\n");
		return;
	}

	// Save workspace
	if (validate)
		save_workspace(&workspace);
}

void remove_workspace()
{
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