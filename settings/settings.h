#ifndef SETTINGS_H
#define SETTINGS_H

void  settings_menu();
void	add_workspace();
void	remove_workspace();

struct web_browser
{
	unsigned int id;
	char name[50];
	char command[65];
};

void  initialize_browsers();
void  list_browsers();
struct web_browser *choose_browser();
struct web_browser *load_selected_browser();
void save_selected_browser(struct web_browser *selected_browser);

void	help(void);
void	usage(void);

#endif // SETTINGS_H