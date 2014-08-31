#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  // PATH_MAX
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>

#define BUF_SIZE 65535

/* types */
typedef struct dirent DIRENT; // dirent.h
typedef struct stat STAT;     // sys/stat.h

/* prototypes */
int check_args(char *);
int open_dir(DIR **, char *);
int close_dir(DIR **, char *);
int processing(char *);
int find(char[], char[], int, int);

/* global */
static int N = 0, ii = 0, mainpid = 0, F = 0;
static int bytes = 0;
static char *program = NULL;
static char curDir[PATH_MAX];
static char m[255];

/* source */
int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "%s: Wrong number of arguments: %d\n", program, argc);
		exit(1);
	}

	/* input data */
	program = &(argv[0][strlen(argv[0]) - strlen(strrchr(argv[0], '/')) + 1]);
	strcpy(m, argv[1]);   //argv[1] - combine of bytes
	strcpy(curDir, argv[2]);   //argv[2] - PATH for search
	N = atoi(argv[3]);   //argv[3] - number of process


	check_args(curDir);
	mainpid = getpid();
	processing(curDir);

	errno = 0;
	while (errno != ECHILD) {
		wait(NULL);
	}

	return 0;

}

int check_args(char *curDir) {
	char buf[PATH_MAX], tmp[50];

	buf[0] = '\0';
	if ((curDir)[strlen(curDir) - 1] == '/' && strlen(curDir) > 1) {
		(curDir)[strlen(curDir) - 1] = '\0';
	}

	if (!strcmp(curDir, ".") || (curDir[0] == '.' && curDir[1] =='/')) {
		getcwd(buf, PATH_MAX);
		strcpy(curDir,&(curDir[1]));
		strcat(buf, curDir);
		strcpy(curDir, buf);
	}

	if (!strcmp(curDir, "..") || (curDir[0] == '.' && curDir[1] == '.' && curDir[2] == '/')) {
		getcwd(buf, PATH_MAX);
		strcpy(curDir, &(curDir[2]));
		strcpy(tmp, curDir);
		strncpy(curDir, buf, strlen(buf) - strlen(strrchr(buf, '/')));
		strcat(curDir, tmp);
	}

	return 0;
}

int open_dir(DIR **dir, char *path) {
	if ((*dir = opendir(path)) == NULL ) {
		fprintf(stderr, "%d: %s: %s: %s\n",getpid(), program, strerror(errno), path);
		errno = 0;
		return -1;
	}
	return 0;
}

int close_dir(DIR **dir, char *path) {
	if (*dir != NULL ) {
		if (closedir(*dir) != 0) {
			fprintf(stderr, "%d: %s: %s: %s\n",getpid(), program, strerror(errno), path);
			errno = 0;
			exit(1);
		}
	}
	return 0;
}

int processing(char *path) {
	DIR *dir = NULL;
	DIRENT *curFile = NULL;
	STAT curInfo;
	char pathName[PATH_MAX];
	int number = 0;

	///////////////////////////////////////////////
	open_dir(&dir, path);
	if (dir == NULL ) {
		return 0;
	}

	errno = 0; // for readdir()

	while ((curFile = readdir(dir)) != NULL) {
		if ((!strcmp((*curFile).d_name, "."))
				|| (!strcmp((*curFile).d_name, ".."))) {
			continue;
		}
		if (!((*curFile).d_type == DT_REG || (*curFile).d_type == DT_DIR)) continue;
		// full PATH
		if (strcmp(path, "/"))
			sprintf(pathName, "%s/%s", path, (*curFile).d_name);
			else sprintf(pathName, "%s%s", path, (*curFile).d_name);

		if (stat(pathName, &curInfo) == 0) {
			// if (dir) then recursive
			if (S_ISDIR(curInfo.st_mode)) {
				processing(pathName);
			} else {
				// if (file) then check
					if (getpid() == mainpid){
						while (ii >= N-1) {
							wait(NULL);
							ii--;
						}
						ii++;
						if (fork() == 0) {
							bytes = 0;
							F = open(pathName, O_RDONLY);
								if (errno != 0){
									fprintf(stderr, "%d: %s: %s: %s\n",getpid(), program, strerror(errno), pathName);
									return -1;
								}
							number = find(m, pathName, curInfo.st_size, F);
							if (number > 0){
								printf("%d: %s %d %d\n",getpid(), pathName, bytes, number);
							}
							close_dir(&dir, path);
							exit(0);
						}
					}
				////////////////////////////////////////////////
				}
		} else {
			fprintf(stderr, "%d: %s: %s: %s\n",getpid(), program, strerror(errno), pathName);
		}
		errno = 0; //for readdir()
	}
	if (errno != 0)   // analyse errno for error in readdir()
			fprintf(stderr, "%d: %s: %s: %s\n",getpid(),  program, strerror(errno), path);

	close_dir(&dir, path);

	return 0;
}

int find(char m[255], char file[PATH_MAX], int size, int F) {
	char buf[BUF_SIZE];
	int flag = 0, j, i;
	int count = 0, result = 0;

	bytes = 0;
	errno = 0;
	count = read(F, buf, BUF_SIZE);
		while (count > 0){		//poka ne konec faila
			for (i = 0; i < strlen(m); i++) {
				if (j >= count){	//esli vyshli za bufer,schitat novyi
					if (bytes < size) count = read(F, buf, BUF_SIZE);
					else {
						close(F);
						return result;
					}
					j = 0;
				}
				if (buf[j] != m[i]) {
					flag = 1;
					break;
				}
				j++;
				bytes++;
			}
			if (!flag) {
				result++;
			} else {
				flag = 0;
				if (i == 0) {
					j++;
					bytes++;
				}
			}
		}

		return result;
}

