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

extern int num_workspaces;
extern struct workspaces *workspaces;

void start_workspace(char *workspace);
void stop_workspace(char *workspace);

#endif // WORKSPACES_H