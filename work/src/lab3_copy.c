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
static int maxN = 0;

/* source */
int main(int argc, char *argv[])
{
    
    char dir1[PATH_MAX], dir2[PATH_MAX];

    if (argc != 4) {
        fprintf(stderr, "%s: Too few arguments: %d\n", program, argc);
        exit(1);
    }

    /* input data */
    program = &(argv[0][(strlen(argv[0]) - strlen(strrchr(argv[0], '/'))) + 1]);
    strcpy(dir1, argv[1]);   //argv[1] - PATH to dir1
    strcpy(dir2, argv[2]);  //argv[2] - PATH to dir2
    maxN = atoi(argv[3]);
    
    //check_args(curDir);
    //printf("%s - %d - %d - %s\n",curDir, s.min, s.max, foutput);
    //printf("\nCount of read files: %d\n", processing(curDir));

    processing(dir1, dir2);

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

int processing(char *path1, char *path2)
{
    
    DIR *dir1, *dir2;
    DIRENT *curPath1, *curPath2;
    STAT curInfo1, curInfo2;
    char pathName1[PATH_MAX], pathName2[PATH_MAX];
    int cmpFlag = 0, N = 0;
    pid_t pid;

    dir1 = dir2 = NULL;
    curPath1 = curPath2 = NULL;
   
    open_dir(&dir1, path1);
    open_dir(&dir2, path2);
    
    if (dir1 == NULL) {
            return 0;
    }

    errno = 0;
    
    while ((curPath1 = readdir(dir1)) != NULL) {
        
        if ((!strcmp((*curPath1).d_name, ".")) || (!strcmp((*curPath1).d_name, ".."))) {
			continue;
		}

        // full PATH
        sprintf(pathName1, "%s/%s", path1, (*curPath1).d_name);        
        if (stat(pathName1, &curInfo1) == 0) {
            if (S_ISDIR(curInfo1.st_mode)) {
                continue;
            }
            else {
                /////////////////////////////////////////////////////////////////////////////////////
                cmpFlag = 0;
                rewinddir(dir2);
                errno = 0;
                
                while ((curPath2 = readdir(dir2)) != NULL) {
                    
                    if ((!strcmp((*curPath2).d_name, ".")) || (!strcmp((*curPath2).d_name, ".."))) {
                        continue;
                    }

                    // full PATH
                    sprintf(pathName2, "%s/%s", path2, (*curPath2).d_name);        
                    if (stat(pathName2, &curInfo2) == 0) {
                        if (S_ISDIR(curInfo2.st_mode)) {
                            continue;
                        }
                        else {
                            /////////////////////////////////////////////////////////////////////////////////////
                            if (strcmp((*curPath1).d_name, (*curPath2).d_name) == 0) {
                                cmpFlag = 1;
                                break;
                            }   
                            //////////////////////////////////////////////////////////////////////////////////////
                        }
                    }
                    else {
                        fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName2);
                    }
                    errno = 0;
                }

                if (errno) {
                    fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName2);
                }
                //////////////////////////////////////////////////////////////////////////////////////
            }
        }
        else {
            fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName2);
        }
        ////////////////////COPYYYYYYYYYYYYYYYYY/////////////////////////
        if (!cmpFlag) {
            if ((pid = fork()) == 0) {
                printf("%d: %s: %ldb\n", getpid(), pathName1, curInfo1.st_size);
                execlp("cp", program, pathName1, path2, NULL);
                break;
            }
            else {

                //printf("COUNT FORKS: %d\n", N);
                
                if (pid > 0) {
                    //waitpid(pid, NULL, 0);
                    if (N >= maxN) {
                        wait(NULL);
                        N--;
                    }
                    else {
                        N++;
                    }
                }
                else {
                    if (pid == -1) {
                        fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName2);
                    }
                }
            }     
        }
        errno = 0;
    }

    if (errno) {
        fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName1);
    }
    
    close_dir(&dir1, path1);
    close_dir(&dir2, path2);
    
    return 0;
}
