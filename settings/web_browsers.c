#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct web_browser
{
	unsigned int id;
	char name[50];
	char command[65];
} *web_browsers;

void initialize_browsers()
{
	web_browsers = malloc(sizeof(struct web_browser) * 5);
	if (web_browsers == NULL)
	{
		perror("Failed to allocate memory for web_browsers");
		exit(EXIT_FAILURE);
	}

	web_browsers[0].id = 1;
	strcpy(web_browsers[0].name, "Firefox");
	strcpy(web_browsers[0].command, "firefox");

	web_browsers[1].id = 2;
	strcpy(web_browsers[1].name, "Chrome");
	strcpy(web_browsers[1].command, "google-chrome");

	web_browsers[2].id = 3;
	strcpy(web_browsers[2].name, "Brave");
	strcpy(web_browsers[2].command, "brave-browser");

	web_browsers[3].id = 4;
	strcpy(web_browsers[3].name, "Opera");
	strcpy(web_browsers[3].command, "opera");

	web_browsers[4].id = 5;
	strcpy(web_browsers[4].name, "Edge");
	strcpy(web_browsers[4].command, "microsoft-edge");
}

void save_selected_browser(struct web_browser *browser)
{
	FILE *file = fopen("selected_browser.dat", "wb");
	if (file == NULL)
	{
		perror("Failed to open file for writing");
		exit(EXIT_FAILURE);
	}
	if(fwrite(browser, sizeof(struct web_browser), 1, file) != 1)
	{
		perror("Failed to write web browser data to file");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	fclose(file);
}

struct web_browser *load_selected_browser()
{
	FILE *file = fopen("selected_browser.dat", "rb");
	if (file == NULL)
	{
		return NULL;
	}
	struct web_browser *browser = malloc(sizeof(struct web_browser));
	if (browser == NULL)
	{
		perror("Failed to allocate memory for browser");
		fclose(file);
		return NULL;
	}
	if (fread(browser, sizeof(struct web_browser), 1, file) != 1)
	{
		perror("Failed to read browser data from file");
		free(browser);
		fclose(file);
		return NULL;
	}
	fclose(file);
	return browser;
}

void list_browsers()
{
	struct web_browser *selected_browser = load_selected_browser();
	if (selected_browser != NULL)
	{
		printf("Current browser: %s\n\n", selected_browser->name);
	}
	printf("Available web browsers:\n");
	for (int i = 0; i < 5; i++)
	{
		printf("%d. %s\n", web_browsers[i].id, web_browsers[i].name);
	}
}

struct web_browser *choose_browser()
{
	unsigned int choice;
	printf("Enter the number of the browser you want to use: ");
	scanf("%d", &choice);

	if (choice < 1 || choice > 5)
	{
		printf("Invalid choice\n");
		return NULL;
	}

	struct web_browser *selected_browser = malloc(sizeof(struct web_browser));
	if (selected_browser == NULL)
	{
		perror("Failed to allocate memory for selected_browser");
		return NULL;
	}

	for (int i = 0; i < 5; i++)
	{
		if (web_browsers[i].id == choice)
		{
			*selected_browser = web_browsers[i];
			free(web_browsers);
			return selected_browser;
		}
	}
	perror("Failed to find browser");
	free(selected_browser);
	return NULL;
}
