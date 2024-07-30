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
extern int num_workspaces;

#endif // WORKSPACES_H