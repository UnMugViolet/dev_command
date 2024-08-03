#include "../program/workspaces.h"
#include "../settings/settings.h"
#include "utils.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void get_user_input(char *buffer, int size)
{
	if (fgets(buffer, size, stdin) == NULL)
	{
		perror("Failed to read user input");
		exit(EXIT_FAILURE);
	}
	buffer[strcspn(buffer, "\n")] = 0;
}

void add_multiple_fields(char **fields, char *input)
{
	printf("Input: %s\n", input);
	char **split_input = str_split(input, ",");
	for (int i = 0; split_input[i]; i++)
	{
		printf("Splited input: %s\n", split_input[i]);
	}

	int input_count = count_words(input, ",");
	printf("Input count: %i\n", input_count);

	for (int i = 0; i < input_count; i++)
	{
		fields[i] = split_input[i];
		printf("%s\n", fields[i]);
	}
	fields[input_count + 1] = NULL;
	printf("Final fields count: %i\n", input_count);
	printf("Manage to go through the loop\n");
}

void process_input(char *input, void *field, int is_array) {
	if (is_array) {
			char ***array_field = (char ***)field;
			*array_field = str_split(input, ",");
	} else {
			char *string_field = (char *)field;
			strncpy(string_field, input, strlen(input) + 1);
	}
}

int count_workspaces()
{
	FILE *file = fopen("workspaces.dat", "rb");
	if (file == NULL)
	{
		return 0;
	}
	struct workspaces workspace;
	int count = 0;
	while (fread(&workspace, sizeof(struct workspaces), 1, file))
	{
		count++;
	}
	fclose(file);
	return count;
}

int	get_workspace_id()
{
	FILE *file = fopen("workspaces.dat", "rb");
	if (file == NULL)
	{
		return 1;
	}

	struct workspaces workspace;
	int max_id = 0;

	while (fread(&workspace, sizeof(struct workspaces), 1, file))
	{
		if ( workspace.id > max_id)
		{
			max_id = workspace.id;
		}
	}
	fclose(file);
	return max_id + 1;
}

int find_workspace_by_id(int *project_id)
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
		if (workspace.id == *project_id)
		{
			fclose(file);
			return workspace.id;
		}
	}
	fclose(file);
	fprintf(stderr, "Workspace %ls not found\n", project_id);
	return -1;
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
	return -1;
}

void list_workspaces() {
	FILE *file = fopen("workspaces.dat", "rb");
	if (file == NULL) {
			printf("No workspaces found.\nworkspace add [name] to add a workspace.\n");
			exit(EXIT_FAILURE);
	}

	struct workspaces workspace;
	while (fread(&workspace, sizeof(struct workspaces), 1, file)) {
			if (workspace.editor != NULL) {
					workspace.editor = malloc(sizeof(struct code_editor));
					fread(workspace.editor, sizeof(struct code_editor), 1, file);
			}

			// Load URLs
			size_t len;
			workspace.urls = NULL;
			while (fread(&len, sizeof(size_t), 1, file) && len > 0) {
					workspace.urls = realloc(workspace.urls, (len + 1) * sizeof(char *));
					workspace.urls[len] = malloc(len);
					fread(workspace.urls[len], sizeof(char), len, file);
			}

			// Load start commands
			workspace.start_command = NULL;
			while (fread(&len, sizeof(size_t), 1, file) && len > 0) {
					workspace.start_command = realloc(workspace.start_command, (len + 1) * sizeof(char *));
					workspace.start_command[len] = malloc(len);
					fread(workspace.start_command[len], sizeof(char), len, file);
			}

			// Load stop commands
			workspace.stop_command = NULL;
			while (fread(&len, sizeof(size_t), 1, file) && len > 0) {
					workspace.stop_command = realloc(workspace.stop_command, (len + 1) * sizeof(char *));
					workspace.stop_command[len] = malloc(len);
					fread(workspace.stop_command[len], sizeof(char), len, file);
			}

			printf("Workspace id: %i\n", workspace.id);
			printf("Workspace name: %s\n", workspace.name);
			printf("Workspace editor: %s\n", workspace.editor->name);
			printf("Workspace path: %s\n", workspace.path);
			printf("URLs: ");
			if (workspace.urls == NULL) {
					printf("None\n");
			} else {
					for (int i = 0; workspace.urls[i]; i++) {
							printf("%s ", workspace.urls[i]);
							printf("\n");
					}
			}
			printf("Start commands: ");
			if (workspace.start_command == NULL) {
					printf("None\n");
			} else {
					for (int i = 0; workspace.start_command[i]; i++) {
							printf("%s ", workspace.start_command[i]);
							printf("\n");
					}
			}
			printf("Stop commands: ");
			if (workspace.stop_command == NULL) {
					printf("None\n");
			} else {
					for (int i = 0; workspace.stop_command[i]; i++) {
							printf("%s ", workspace.stop_command[i]);
							printf("\n");
					}
			}
			printf("Need sudoer: %d\n", workspace.need_sudoer);
			printf("\n");
	}
	fclose(file);
}