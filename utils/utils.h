#ifndef UTILS_H
#define UTILS_H

char **str_split(char *str, char *charset);
void clear_input_buffer();

void list_workspaces();
int get_workspace_id();
int find_workspace(char *workspace_name);
int find_workspace_by_id(int *workspace_id);
int count_workspaces();

#endif // UTILS_H