struct workspaces
{
	unsigned int id;
	char name [50];
	char path [120];
	char **urls;
	char *launch_command;
	char *stop_command;
	unsigned int need_sudoer;
} *workspaces;

void	start_project()
{}

void	stop_project()
{}
