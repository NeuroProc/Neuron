#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define MAXELEM 100

typedef struct file TFile;
struct file {
	char *path;
	TFile *next;
};

char *EditArgv(char *, char *);
TFile *SearchFile(char *, char *, int *, int *, TFile *);
char *EditAdress(char *, char *, char *);
void PrintFile(char *, char *, int, int);
void GetRights(mode_t, char *);
int CheckErrors(int, char *[]);
TFile *AddToList(TFile *, char *);


int main(int argc, char *argv[])
{
	char *way_to_file;
	char *parent_catalog = NULL;
	int catalog_counter = 1, file_counter = 0;
	TFile *file_list = NULL;
	
	//check for errors, if found - exit program
	if (CheckErrors(argc, argv))
		return 1;

	//edit parent path(if the last letter = '/' - delete it
	parent_catalog = EditArgv(parent_catalog, argv[1]);
	
	//recursive function of searching file. if file wasn`t found - exit
	if ((file_list = SearchFile(parent_catalog, argv[2], &catalog_counter, &file_counter, file_list)) == NULL) {
		fprintf(stderr, "%s: Such file not found...\n", argv[1]);
		return 0;
	} 
	
	//print the information about file
	PrintFile(way_to_file, argv[2], catalog_counter, file_counter);
	while(file_list != NULL) {
		printf("\n%s\n", file_list->path);
		file_list = file_list->next;
	}
	
	return 0;
}

int CheckErrors(int argc, char *argv[])
{
	if (argc != 3) {
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
	if (argv[strlen(argv) -1] != '/') { 
		parent_catalog = (char *) malloc(strlen(argv) + 1);
		strcpy(parent_catalog, argv);
	}
	else {
		parent_catalog = (char *) malloc(strlen(argv) + 1);
		strcpy(parent_catalog, argv);
		parent_catalog[strlen(argv) - 1] = '\0';
	}
	
	return parent_catalog;
}

TFile *SearchFile(char *catalog_name, char *file_to_search, int *catalog_counter, int *file_counter, TFile *file_list)
{
	DIR *current_catalog;
	struct dirent *current_element;
	char *current_way = NULL;
	static int flag = TRUE;
	static int count = 0;

	current_catalog = opendir(catalog_name);
	
	while ((current_element = readdir(current_catalog)) != NULL && flag) {
		if ((strcmp(current_element->d_name,"..") != 0) && (strcmp(current_element->d_name,".") != 0)) {
			current_way = EditAdress(current_way, catalog_name, current_element->d_name);
			if (opendir(current_way) != NULL) {
				(*catalog_counter)++;
				file_list = SearchFile(current_way, file_to_search, catalog_counter, file_counter, file_list);
			}
			else {
				printf("%s\n", current_way);
				(*file_counter)++;
				if (strcmp(current_element->d_name, file_to_search) == 0) {
					file_list = AddToList(file_list, current_way);
					count++;
				} 
			}
		}		
	}
	
	return file_list;
}

TFile *AddToList(TFile *file_list, char *path)
{
	TFile *new_file;
	
	new_file = (TFile *) malloc(sizeof(TFile));
	new_file->path = (char *) malloc(strlen(path) + 1);
	strcpy(new_file->path, path);
	new_file->next = file_list;
	
	return new_file;
}

char *EditAdress(char *current_way, char *catalog_name, char *file_name)	
{
	current_way = (char *) malloc(strlen(catalog_name) + strlen(file_name) + 2);
	strcpy(current_way, catalog_name);
	current_way[strlen(catalog_name)] = '/';
	current_way[strlen(catalog_name) + 1] = '\0';
	strcat(current_way, file_name);

	return current_way;
}

void PrintFile(char *way_to_file, char *name_of_file, int catalog_counter, int file_counter)
{
	struct stat stats_of_file;
	char rights[11];

	printf("\n==> Found file %s in %s\n\n", name_of_file, way_to_file);
	printf("Total catalogs looked through: %d\n", catalog_counter);
	printf("Total files looked through: %d\n\n", file_counter);

	stat(way_to_file, &stats_of_file);
	
	//print rights in (char *) mode
	GetRights(stats_of_file.st_mode, rights);
	
	printf("%s  %d  %s", name_of_file, stats_of_file.st_size, rights);
	printf("  %.12s  %d\n", 4 + ctime(&(stats_of_file.st_ctime)), stats_of_file.st_ino);	
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

