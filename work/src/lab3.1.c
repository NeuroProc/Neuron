#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  // PATH_MAX
#include <errno.h>
#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

/* defines */
#define BUFLEN 65536

/* types */
typedef struct dirent DIRENT; // dirent.h
typedef struct stat STAT;     // sys/stat.h

/* prototypes */
int open_dir(DIR **, char *);
int close_dir(DIR **, char *);
int processing(char *, char *);
int copy(char *, char *, mode_t);

/* global */
static char *program = NULL;
static int maxN = 0, N = 0;
static char *cmpDir = NULL;

/* source */
int main(int argc, char *argv[])
{
    
    char dir1[PATH_MAX];

    if (argc != 4) {
        fprintf(stderr, "%d: Too few arguments: %d\n", getpid(), argc);
        exit(1);
    }

    /* input data */
    //program = &(argv[0][(strlen(argv[0]) - strlen(strrchr(argv[0], '/'))) + 1]);
    program = basename(argv[0]);
    strcpy(dir1, argv[1]);   //argv[1] - PATH to dir1
    //strcpy(dir2, argv[2];  //argv[2] - PATH to dir2
    cmpDir = argv[2];
    maxN = atoi(argv[3]);

    //check_args(curDir);
    //printf("%s - %d - %d - %s\n",curDir, s.min, s.max, foutput);
    //printf("\nCount of read files: %d\n", processing(curDir));

    processing(dir1, NULL);

    errno = 0;
    while (errno != ECHILD)
        wait(NULL);
    
    return 0;

}


int open_dir(DIR **dir, char *path)
{
    if ((*dir = opendir(path)) == NULL) {
        fprintf(stderr, "%d: %s: %s\n", getpid(), strerror(errno), path);
        //exit(1);
        return -1;
    }
    return 0;
}

int close_dir(DIR **dir, char *path)
{
    if (*dir != NULL) {
        if (closedir(*dir) != 0) {
            fprintf(stderr, "%d: %s: %s\n", getpid(), strerror(errno), path);
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
    char copyFile[PATH_MAX];

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
            fprintf(stderr, "%d: %s: %s\n", getpid(), strerror(errno), pathName);
        }
        ////////////////////COPYYYYYYYYYYYYYYYYY/////////////////////////
        if (!cmpFlag && (cmpFile == NULL)) {
            /////////////////////////
            while (N >= maxN) {
                if (wait(NULL)  == -1) {
                    fprintf(stderr, "%d: %s: %s", getpid(), program, strerror(errno));
                }
                N--;
            }
            ///////////////////////////
            switch (fork()) {
                case 0: {
                    if (!S_ISDIR(curInfo.st_mode)) {
                        sprintf(copyFile, "%s/%s", cmpDir, (*curPath).d_name);
                        if (copy(pathName, copyFile, curInfo.st_mode) != -1) {
                            printf("%d: %s: %ld\n", getpid(), pathName, curInfo.st_size);
                        }
                    }
                    /*
                    sprintf(pid, "%d", getpid());
                    if (execlp("cp", pid, pathName, cmpDir, NULL) == -1) {
                        fprintf(stderr, "%d: %s: %s", getpid(), strerror(errno), pathName);
                    }   
                    */
                    sleep(5);
                    exit(0);
                    //break;
                }
                case -1: {
                    fprintf(stderr, "%d: %s: %s\n", getpid(), strerror(errno), pathName);
                    break;
                }
                default: {
                    N++;
                }
            }
        }
        ////////////////////COPYYYYYYYYYYYYYYYYY/////////////////////////
        errno = 0;
    }

    if (errno) {
        fprintf(stderr, "%d: %s: %s\n", getpid(), strerror(errno), pathName);
    }
    
    close_dir(&dir, path);
    
    return 0;
}

int copy(char *x, char *y, mode_t mode) {
    char buf[BUFLEN];
    char cntlRep[PATH_MAX];
    int xD = 0, yD = 0, cntlCount = 0;
    long count = 0;

    //printf("%s TO %s\n",x,y);
    
    if ((xD = open(x, O_RDONLY)) == -1) {
        fprintf(stderr, "%d: %s\n", getpid(), strerror(errno));
        return -1;
    }

    sprintf(cntlRep, "%s", y);
    while (access(cntlRep, F_OK) == 0) {
        sprintf(cntlRep, "%s(%d)", y, cntlCount++);
    }

    if ((yD = open(cntlRep, O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
        fprintf(stderr, "%d: %s\n", getpid(), strerror(errno));
        return -1;
    }

    while ((count = read(xD, buf, BUFLEN)) > 0) {
        if (count == -1) {
            fprintf(stderr, "%d: %s\n", getpid(), strerror(errno));
            return -1;
        }

        if (write(yD, buf, count) == -1) {
            fprintf(stderr, "%d: %s\n", getpid(), strerror(errno));
            return -1;
        }
    }

    if (fchmod(yD, mode) == -1) {
        fprintf(stderr, "%d: %s\n", getpid(), strerror(errno));
        return -1;
    }   

    if (close(xD) == -1) {
        fprintf(stderr, "%d: %s\n", getpid(), strerror(errno));
        return -1;
    }
    if (close(yD) == -1) {
        fprintf(stderr, "%d: %s\n", getpid(), strerror(errno));
        return -1;
    }
        

    return 0;
}
                
            
    
    

    
