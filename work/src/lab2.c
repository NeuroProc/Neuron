/***********************************************************
 *  file: lab2.c
 *  compiler: clang
 *  author: Neuron
 *  usage: lab2 path output_file min_size max_size min_date max_date
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  // PATH_MAX
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

/* defines */
#define MAXLEN 40

/* types */
struct eval {
    off_t sMin, sMax;  // min & max sizes 
    time_t dMin, dMax;  // min & max dates of create
};

typedef struct eval EVAL;
typedef struct dirent DIRENT; // dirent.h
typedef struct stat STAT;     // sys/stat.h

/* prototypes */
int check_args(char []);
int open_dir(DIR **, char *);
int close_dir(DIR **, char *);
int processing(char *);
int inFile(char *);
time_t code_date(char []);
char *decode_date(time_t);

/* global */
static EVAL data;
static char foutput[PATH_MAX];
static int fdesc = 0;
static char *program = NULL;

/* source */
int main(int argc, char *argv[])
{
    char curDir[PATH_MAX];
    /*
    if (argc != 7) {
        fprintf(stderr, "%s: Too few arguments: %d\n", program, argc);
        exit(1);
    }
    */

    /* input data */
    program = &(argv[0][(strlen(argv[0]) - strlen(strrchr(argv[0], '/'))) + 1]);
    strcpy(curDir, argv[1]);   //argv[1] - PATH for search
    strcpy(foutput, argv[2]);  //argv[4] - PATH to output file
    /* sizes */
    data.sMin = atoi(argv[3]); 
    data.sMax = atoi(argv[4]);
    /* dates of create */
    data.dMin = code_date(argv[5]);
    data.dMax = code_date(argv[6]);

    printf("%ld - %ld\n",data.dMin, data.dMax);

/*    

    if ((fdesc = open(foutput, O_WRONLY)) == -1) {
        fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), foutput);
    }
    
    check_args(curDir);
    //printf("%s - %d - %d - %s\n",curDir, s.min, s.max, foutput);

    //printf("\nCount of read files: %d\n", processing(curDir));

    processing(curDir);


    close(fdesc);
*/
    return 0;

}

time_t code_date(char date[])
{
    struct tm t;

    /* separate string */
    t.tm_mday = atoi(strtok(date, "."));
    t.tm_mon = atoi(strtok(NULL, ".")) - 1;
    t.tm_year = atoi(strtok(NULL, ".")) - 1900;

    return mktime(&t);
}

char *decode_date(time_t date)
{
    static char buf[MAXLEN];
    struct tm *t = localtime(&date);
    
    sprintf(buf, "%d.%d.%d %d:%d:%d", (*t).tm_mday, (*t).tm_mon + 1, (*t).tm_year + 1900, (*t).tm_hour, (*t).tm_min, (*t).tm_sec);

    return buf;
}

int check_args(char curDir[PATH_MAX])
{
    char buf[PATH_MAX];

    if (curDir[strlen(curDir) - 1] == '/') {
        curDir[strlen(curDir) - 1] = '\0';
    }

    if (strcmp(curDir, ".") == 0) {
        getcwd(curDir, PATH_MAX);
    }
	
    if (strcmp(curDir, "..") == 0) {
        getcwd(curDir, PATH_MAX);
        strncpy(buf, curDir, strlen(curDir) - strlen(strrchr(curDir, '/')));
        strcpy(curDir, buf);
    }

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

int processing(char *path)
{
    DIR *dir = NULL;
    DIRENT *curPath = NULL;
    STAT curInfo;
    char pathName[PATH_MAX], *buf = NULL;
    int numFiles = 0;


    ////////////////////////////////////////////////
    //FILE *fo;
    //fo = fopen("/home/Neuron/work/lol.txt","w");
    ///////////////////////////////////////////////

    open_dir(&dir, path);
    if (dir == NULL) {
        return numFiles;	
    }
        
    while ((curPath = readdir(dir)) != NULL) {

        if ((int)curPath == -1) {
            fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName);
        }
            
        if ((!strcmp((*curPath).d_name, ".")) || (!strcmp((*curPath).d_name, ".."))) {
			continue;
		}
        
        // full PATH
        sprintf(pathName, "%s/%s", path, (*curPath).d_name);
        
        if (stat(pathName, &curInfo) == 0) {
            // if (dir) then recursive
            if (S_ISDIR(curInfo.st_mode)) {
                //printf("PATH: %s\n", pathName);
                numFiles += processing(pathName);
            }
            else {

                printf("%s - %o\n", (*curPath).d_name, curInfo.st_mode);
                /////////////////////////////////////////////////////////////////////////////////////
                /* inc count of files */
                numFiles++;
                
                if  (curInfo.st_size >= data.sMin && curInfo.st_size <= data.sMax
                    && curInfo.st_ctime >= data.dMin && curInfo.st_ctime <= data.dMax) {
                    
                    buf = malloc(strlen(pathName) + strlen((*curPath).d_name) + MAXLEN);
                    
                    sprintf(buf, "%s %lu %s\n", pathName, curInfo.st_size, decode_date(curInfo.st_ctime));
                    printf("%s", buf);
                    inFile(buf);
                    
                    
                    free(buf);
                }
                //////////////////////////////////////////////////////////////////////////////////////
            }
        }
        else {
            fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), pathName);
        }
    }
    close_dir(&dir, path);
    
    return numFiles;
}

int inFile(char *buf)
{
    //fputc(buf[i], fdesc);
    //printf("LOL\n");
    write(fdesc, buf, strlen(buf));

    return 0;
}
