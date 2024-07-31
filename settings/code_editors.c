#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct code_editor
{
	unsigned int id;
	char name[50];
	char command[65];
} *code_editors;

const unsigned int code_editors_count = 7;

void initialize_code_editor()
{
	code_editors = malloc(sizeof(struct code_editor) * code_editors_count);
	if (code_editors == NULL)
	{
		perror("Failed to allocate memory for code_editors");
		exit(EXIT_FAILURE);
	}

	code_editors[0].id = 1;
	strcpy(code_editors[0].name, "Vim");
	strcpy(code_editors[0].command, "gnome-terminal");

	code_editors[1].id = 2;
	strcpy(code_editors[1].name, "Visual Studio Code");
	strcpy(code_editors[1].command, "code .");

	code_editors[2].id = 3;
	strcpy(code_editors[2].name, "Sublime Text");
	strcpy(code_editors[2].command, "subl .");

	code_editors[3].id = 4;
	strcpy(code_editors[3].name, "IntelliJ WebStorm");
	strcpy(code_editors[3].command, "webstorm .");

	code_editors[4].id = 5;
	strcpy(code_editors[4].name, "IntelliJ IDEA");
	strcpy(code_editors[4].command, "idea .");

	code_editors[5].id = 6;
	strcpy(code_editors[5].name, "IntelliJ PyCharm");
	strcpy(code_editors[5].command, "pycharm .");

	code_editors[6].id = 7;
	strcpy(code_editors[6].name, "Atom");
	strcpy(code_editors[6].command, "atom .");
}

struct code_editor *load_selected_code_editor()
{
	FILE *file = fopen("selected_code_editor.dat", "rb");
	if (file == NULL)
	{
		return NULL;
	}
	struct code_editor *editor = malloc(sizeof(struct code_editor));
	if (editor == NULL)
	{
		perror("Failed to allocate memory for code editor");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	if (fread(editor, sizeof(struct code_editor), 1, file) != 1)
	{
		perror("Failed to read code editor data from file");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	fclose(file);
	return editor;
}

void list_code_editors()
{
	printf("0. None\n");
	for (unsigned int i = 0; i < code_editors_count; i++)
	{
		printf("%d. %s\n", code_editors[i].id, code_editors[i].name);
	}
}

struct code_editor *choose_code_editor()
{
	initialize_code_editor();
	list_code_editors();
	printf("Enter the ID of the code editor you want to use: ");
	unsigned int choice;
	scanf("%u", &choice);

	if (choice > code_editors_count)
	{
		printf("Invalid choice\n");
		return NULL;
	}

	struct code_editor *selected_editor = malloc(sizeof(struct code_editor));
	if (selected_editor == NULL)
	{
		perror("Failed to allocate memory for the code editor\n");
		return NULL;
	}

	for (unsigned int i = 0; i < code_editors_count; i++)
	{
		if (choice == 0)
		{
			return NULL;
		}
		if (code_editors[i].id == choice)
		{
			*selected_editor = code_editors[i];
			return selected_editor;
		}
	}
	printf("Failed to find the code editor\n");
	free(selected_editor);
	return NULL;
}
