#include <stdio.h>

#ifndef UTILS_H
#define UTILS_H

char **str_split(char *str, char *charset);
char **deserialize_array(FILE *file);

// Input functions
void get_user_input(char *buffer, int size);
void add_multiple_fields(char **fields, char *input);
void process_input(char *input, void *field, int is_array);

// String treatmen functions
int count_words(char *str, char *charset);

// Workspace functions
int count_workspaces();
int get_workspace_id();
int find_workspace_by_id(int *workspace_id);
int find_workspace(char *workspace_name);
char **deserialize_array(FILE *file);
struct workspaces *read_workspace(FILE *file);
void list_workspaces();

#endif // UTILS_H