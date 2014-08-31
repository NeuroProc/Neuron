#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define MAXELEM 100

char *EditArgv(char *, char *);
void SearchFile(char *, char *[]);
char *EditAdress(char *, char *);
void PrintFile(char *, char *, int, int);
void GetRights(mode_t, char *);
int CheckErrors(int, char *[]);
void PrintInf(char *[], int, int);

static int pr_c = 0;
static int success = FALSE;

int main(int argc, char *argv[])
{
	char *parent_catalog = NULL;
	int i = 0, file_counter = 0;
	pid_t ppid;
	
	/*check for errors, if found - exit program*/
	if (CheckErrors(argc, argv))
		return 1;

	/*edit parent path(if the last letter == '/' - delete it)*/
	parent_catalog = EditArgv(parent_catalog, argv[1]);
	
	/*recursive function of searching file. if file wasn`t found - exit*/
	SearchFile(parent_catalog, argv);

	return 0;
}

void PrintInf(char *argv[], int catalog_counter, int file_counter)
{	
	if (!success) 
		fprintf(stderr, "\n%s: Such file not found...", argv[1]);
	printf("\nTotal catalogs looked through: %d\n", catalog_counter);
	printf("Total files looked through: %d\n\n", file_counter);
}

int CheckErrors(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(stderr, "%s: Error: Wrong amount of arguments...\n", argv[0]);
		return 1;
	}
	if (opendir(argv[1]) == NULL) {
		fprintf(stderr, "%s: Error: No such catalog %s\n", argv[0], argv[1]);
		return 1;
	}
	return 0;
}

char *EditArgv(char *parent_catalog, char *argv)
{
	if (argv[strlen(argv) -1] != '/' || strlen(argv) == 1) { 
		parent_catalog = (char *) malloc(strlen(argv) + 1); //1
		strcpy(parent_catalog, argv);
	}
	else {
		parent_catalog = (char *) malloc(strlen(argv) + 1); //1
		strcpy(parent_catalog, argv);
		parent_catalog[strlen(argv) - 1] = '\0';
	}
	
	return parent_catalog;
}

void SearchFile(char *catalog_name, char *argv[])
{
	DIR *current_catalog;
	struct dirent *current_element;
	char *current_way = NULL;
	int proc_count = atoi(argv[3]);
	int state;
	int perem;
	pid_t pid;
	errno = 0;
	
	/*if the directory can be opened*/
	if (current_catalog = opendir(catalog_name)) {
		while(TRUE) {
			current_element = readdir(current_catalog);
			if (current_element)
				if ((strcmp(current_element->d_name,"..") != 0) && (strcmp(current_element->d_name,".") != 0)) {
					/*Add new element`s name to the path*/
free(current_way);					
current_way = EditAdress(catalog_name, current_element->d_name);
					if (current_element->d_type == 4) 
						SearchFile(current_way, argv);
					else
						if (current_element->d_type == 8) {
							state = TRUE;
							if (pr_c < proc_count) 
								pr_c++;
							else {
								wait();
								pr_c--;
							}
							pr_c++;
						
							pid = fork();
							if (pid == 0) {
								printf("%s\n", current_way);
								exit(0);
							}
							success = TRUE; 
						}
			}
			/*if the error, when openenig file, occured - pass it*/
			if (errno) {
				fprintf(stderr, "%s: %s: %s\n", argv[0], current_way, strerror(errno));
				errno = 0; 
			}
			/*leave when reached catalog`s end*/
			else
				if (current_element == NULL)
					break;

		}
		if (closedir(current_catalog) == -1)
			fprintf(stderr, "%s: %s: Couldn`t close catalog\n", argv[0], catalog_name);

	}
	else 
		fprintf(stderr, "%s: %s: Couldn`t open catalog\n", argv[0], catalog_name);	
	
	
}

char *EditAdress(char *catalog_name, char *file_name)	
{
	char *current_way = NULL;

	current_way = (char *) malloc(strlen(catalog_name) + strlen(file_name) + 2);//2
	strcpy(current_way, catalog_name);
	if(strlen(catalog_name) != 1)
		current_way[strlen(catalog_name)] = '/';
	current_way[strlen(catalog_name) + 1] = '\0';
	strcat(current_way, file_name);

	return current_way;
}

void PrintFile(char *way_to_file, char *name_of_file, int catalog_counter, int file_counter)
{
	char dat[5];
	struct tm *local = NULL;
	struct stat stats_of_file;
	char rights[11];

	printf("\n%s", way_to_file);
	
	stat(way_to_file, &stats_of_file);
	GetRights(stats_of_file.st_mode, rights);
	local = localtime(&stats_of_file.st_ctime);

	printf("  %d  %s", stats_of_file.st_size, rights);
	printf("  %d.0%d.%d", local->tm_mday, local->tm_mon, local->tm_year + 1900);
	printf("  %d", stats_of_file.st_ino);	
}

void GetRights(mode_t mode, char *rights)
{	
	strcpy(rights, "----------");
	if (mode & S_IRUSR)
		rights[1] = 'r';
	if (mode & S_IWUSR)
		rights[2] = 'w';
	if (mode & S_IXUSR)
		rights[3] = 'x';
	if (mode & S_IRGRP)
		rights[4] = 'r';
	if (mode & S_IWGRP)
		rights[5] = 'w';
	if (mode & S_IXGRP)
		rights[6] = 'x';
	if (mode & S_IROTH)
		rights[7] = 'r';
	if (mode & S_IWOTH)
		rights[8] = 'w';
	if (mode & S_IXOTH)
		rights[9] = 'x';
}
