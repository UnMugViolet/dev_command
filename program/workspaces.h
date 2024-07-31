#ifndef WORKSPACES_H
#define WORKSPACES_H

struct workspaces {
    int id;
    char name[60];
    char path[120];
    struct code_editor *editor;
    char **urls;
    char **start_command;
    char **stop_command;
    unsigned int need_sudoer;
};

extern struct workspaces *workspaces;

void start_workspace(char *workspace);
void stop_workspace(char *workspace);
void list_workspaces();
int get_workspace_id();
int find_workspace(char *workspace_name);
int find_workspace_by_id(int *workspace_id);

#endif // WORKSPACES_H