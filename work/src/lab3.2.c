#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  // PATH_MAX
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>

/* types */
typedef struct dirent DIRENT; // dirent.h
typedef struct stat STAT;     // sys/stat.h

/* prototypes */
int open_dir(DIR **, char *);
int close_dir(DIR **, char *);
int processing(char *, char *);

/* global */
static char *program = NULL;
static int maxN = 0, N = 0;
static char *cmpDir = NULL;

/* source */
int main(int argc, char *argv[])
{
    
    char dir[PATH_MAX];

    if (argc != 4) {
        fprintf(stderr, "%s: Too few arguments: %d\n", program, argc);
        exit(1);
    }

    /* input data */
    program = &(argv[0][(strlen(argv[0]) - strlen(strrchr(argv[0], '/'))) + 1]);
    strcpy(dir, argv[1]);   //argv[1] - PATH to dir1
    //strcpy(dir2, argv[2];  //argv[2] - PATH to dir2
    cmpDir = argv[2];
    maxN = atoi(argv[3]);

    //check_args(curDir);
    //printf("%s - %d - %d - %s\n",curDir, s.min, s.max, foutput);
    //printf("\nCount of read files: %d\n", processing(curDir));

    processing(dir, NULL);

    return 0;

}


int open_dir(DIR **dir, char *path)
{
    if ((*dir = opendir(path)) == NULL) {
        fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), path);
        //exit(1);
        return -1;
    }
    return 0;
}

int close_dir(DIR **dir, char *path)
{
    if (*dir != NULL) {
        if (closedir(*dir) != 0) {
            fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), path);
            exit(1);		
        }
    }
    return 0;
}

int processing(char *path, char *cmpFile)
{    
    DIR *dir = NULL;
    DIRENT *curPath = NULL;
    STAT curInfo;
    char pathName[PATH_MAX];
    int cmpFlag = 0;
    pid_t pid = 0;

    open_dir(&dir, path);
    
    if (dir == NULL) {
            return 0;
    }

    errno = 0;
    
    while ((curPath = readdir(dir)) != NULL) {

        cmpFlag = 0;
        
        if ((!strcmp((*curPath).d_name, ".")) || (!strcmp((*curPath).d_name, ".."))) {
			continue;
		}

        // full PATH
        sprintf(pathName, "%s/%s", path, (*curPath).d_name);        
        if (stat(pathName, &curInfo) == 0) {
            if (S_ISDIR(curInfo.st_mode)) {
                processing(pathName, cmpFile);
            }
            else {
                /////////////////////////////////////////////////////////////////////////////////////
                if (cmpFile == NULL) {
                    cmpFlag = processing(cmpDir, (*curPath).d_name);
                }
                else {
                    if (strcmp((*curPath).d_name, cmpFile) == 0) {
                        close_dir(&dir, path);
                        return 1;
                    }
                }
                  
                //////////////////////////////////////////////////////////////////////////////////////
            }
        }
        else {
            fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName);
        }
        ////////////////////COPYYYYYYYYYYYYYYYYY/////////////////////////
        if (!cmpFlag && (cmpFile == NULL)) {
             switch (fork()) {
                case 0: {
                    printf("%d: %s: %ldb\n", getpid(), pathName, curInfo.st_size);
                    execlp("cp", program, pathName, cmpDir, NULL);
                    return 0;
                    //break;
                }
                case -1: {
                    fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName);
                    break;
                }
                default: {
                    if (N >= maxN) {
                        errno = 0;
                        while (errno != ECHILD) {
                            wait(NULL);
                        }
                        N = 0;
                    }
                    else {
                        N++;
                    }
                }
            }
                    
            
        }
        ////////////////////COPYYYYYYYYYYYYYYYYY/////////////////////////
        errno = 0;
    }

    if (errno) {
        fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName);
    }
    
    close_dir(&dir, path);
    
    return 0;
}
