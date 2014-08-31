#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>

/* defines */
#define BUFLEN 1
#define BUFFER 200
#define MINLEN 7  // >
#define MINSEPS 3   // >
#define BORDERLEN 15  // >

/* types */
typedef struct stat STAT;     // sys/stat.h
typedef struct dirent DIRENT; // dirent.h
typedef struct tArgs {
    int i;
    char path[PATH_MAX];
} TARGS; //arguments of thread function

struct note {
    long start, len;
    struct note *next;
};
typedef struct note NOTE;
typedef NOTE *PNOTE;

/* prototypes */
void *threadF(void *);
int open_dir(DIR **, char *);
int close_dir(DIR **, char *);
int processing(char *);
int findMatch(char *, char *, long, long, long, long, long, char *);
long getBuf(int, char *, long, int);
long putBuf(int, char *, long);
int errOpen(char *, int);
int errClose(int);
long fileSize(char *path);
PNOTE push(long, long);
void destroy(PNOTE);
int find(PNOTE, long);

/* global */
static char *program = NULL;
static char *resFile = "result.txt";
static char *inFile = NULL;
static char *sepStr = " \n\t\v\r";
static long N = 0;
static int resFD;
pthread_t *tidMas = NULL;
pthread_mutex_t mutex;
static int tCounter = 0, jj = 0;
struct timespec ts; 

int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "%s: Too few arguments: %d\n", program, argc);
        exit(1);
    }

/*
* argv[0] - program
* argv[1] - input file
* argv[2] - output path
* argv[3] - number of threads
*/

    /* input data */
    program = basename(argv[0]);
    inFile = argv[1];
    N = atoi(argv[3]);

    tidMas = malloc(sizeof(pthread_t) * N);
    unlink(resFile);

    ts.tv_sec = 0;
    ts.tv_nsec = 1;
    
    /* open files */
    //inFD = errOpen(inFile, O_RDONLY);
    resFD = errOpen(resFile, O_WRONLY | O_CREAT | O_TRUNC);


    /* create mutex */
    if (pthread_mutex_init(&mutex, NULL) == -1) {
        fprintf(stderr, "%s: %s\n", program, strerror(errno));
        exit(errno);
    }

    processing(argv[2]);
    getchar();
    
    /* destroy mutex */
    if (pthread_mutex_destroy(&mutex) == -1) {
        fprintf(stderr, "%s: %s\n", program, strerror(errno));
        exit(errno);
    }
   
    /* close files */
    //errClose(inFD);
    errClose(resFD);
    
    return 0;
}

void *threadF(void *arg) {

    TARGS *tmp = (TARGS *)arg;
    char outBuf, inBuf, tmpBuf, resBuf[BUFFER];
    long outCount, inCount;
    long localFUS = 0, len = 0;
    long i = 0, j = 0;
    int outFD, inFD;
    PNOTE head, cur;
    
    /* init list */
    head = push(-1, -1);
    cur = head;

    //printf("%ld: %ld\n",(*tmp).i, globPos);

    /* open files */
    outFD = errOpen((*tmp).path, O_RDONLY);
    inFD = errOpen(inFile, O_RDONLY);
    
    /* read inFile */
    inCount = getBuf(inFD, &inBuf, BUFLEN, 1);
    i = 0;
    while (inCount > 0) {
        // read outFile

        if (find(head, i) == 1) goto CONT;
        
        lseek(outFD, 0, SEEK_SET);
        outCount = getBuf(outFD, &outBuf, BUFLEN, 0);
        j = 0;
        len = 0;
        tmpBuf = inBuf;
        
        //printf("INCOUNT: %ld     OUTCOUNT: %ld\n", inCount, outCount);
        
        while (outCount > 0) {

            //printf("%c - %c\n",inBuf, outBuf);
            if (inBuf == outBuf && inCount > 0) {
                len++;
                inCount = getBuf(inFD, &inBuf, BUFLEN, 1);
                //printf("%ld",inCount);
                if (inCount == 0) goto POINT;
                
            } else {
                //printf("%s : %ld : %ld : %ld\n", (*tmp).path, i,j,len);
              POINT:
                if (len >= BORDERLEN) {
                    sprintf(resBuf, "%lu: %s: %ld: %ld: %ld\n", pthread_self(),(*tmp).path, i, j, len);
                    putBuf(resFD, resBuf,strlen(resBuf));

                    (*cur).next = push(i, len);
                    cur = (*cur).next;

                }
                
                if (len > 0) {
                    lseek(inFD, i, SEEK_SET);
                    inBuf = tmpBuf;
                    len = 0;
                }
            }

            //localFUS = findMatch(inBuf, outBuf, inCount, outCount, localFUS, i * BUFLEN, j * BUFLEN, (*tmp).path);
            outCount = getBuf(outFD, &outBuf, BUFLEN, 0);
            j++;
            //printf("J:%d ; OUTCOUNT:%ld\n",j, outCount);
        }
      CONT:
        inCount = getBuf(inFD, &inBuf, BUFLEN, 1);
        i++;
        //printf("I:%d ; INCOUNT:%ld\n",i, inCount);
    }

    //getchar();
    errClose(inFD);
    errClose(outFD);
    free(arg);
    destroy(head);
    //usleep(500000);
    pthread_exit("EXIT");
}

PNOTE push(long start, long len) {
    PNOTE res = NULL;

    res = (PNOTE)malloc(sizeof(NOTE));
    (*res).start = start;
    (*res).len = len;
    (*res).next = NULL;

    return res;
}

void destroy(PNOTE p) {
    PNOTE tmp;
    
    while (p != NULL) {
        tmp = p;
        p = (*p).next;
        free(tmp);
    }
}

int find(PNOTE p, long i) {

    while (p != NULL) {
        if ( (i >= (*p).start) && (i <= (*p).start + (*p).len) )
            return 1;
        p = (*p).next;
    }
    return 0;
}


int findMatch(char *inBuf, char *outBuf, long inCount, long outCount, long fus, long out, long in, char *path) {
    long i, j, sepCounter, ii;
    char resBuf[BUFLEN];
    char tmpBuf[BUFLEN];


    //printf("!!! INBUF:\n%s\n!!! OUTBUF:\n%s\n",inBuf, outBuf);
    //printf("FUS=%ld\n",fus);
    
    
    for (i = 0; i < inCount; i++) {
        ii = i;
        //sepCounter = 0;
        for (j = 0; j < outCount; j++) {
            if (inBuf[ii] == outBuf[j]) {

                //printf("ii=%ld == j=%ld\n",ii,j);
                
                ii++; // INC

                if (ii == inCount) {
                    //printf("FUS DETECTED! -- %ld\n", ii-i);
                    //return fus+ii-i;
                    goto M1;
                }

                /* PROST */
/*
                if (strchr(sepStr, outBuf[j]) != NULL) {
                    sepCounter++;
                }
*/                
            } else {
              M1:
                if (ii-i > BORDERLEN) {
                    
                    //printf("START: i=%ld ; j=%ld ii=%ld fus=%ld\n",i , j, ii,fus);
                    
                    ////////////////////////////////////////
                    //printf("NASHEL!!!");
                    char tmpbuf[ii-i];
                    strncpy(tmpbuf, inBuf+i, ii-i);
                    tmpbuf[ii-i] = '\0';
                    //printf("%ld: N - %ld: D -  %ld: %s\n", pthread_self(), i, ii-i, tmpbuf);
                    ////////////////////////////////////////
                    
                    sprintf(resBuf, "%lu: %s: %ld: %ld: %ld\n",pthread_self(), path, in+i, out+j+11-(ii-i), ii-i);
                    putBuf(resFD, resBuf, strlen(resBuf));

                    //putBuf(resFD, tmpbuf, ii-i);

                    /* delete match */
                    strcpy(tmpBuf, outBuf);
                    memset(outBuf, 0, BUFLEN);
                    tmpBuf[i] = '\0';
                    sprintf(outBuf, "%s%s", tmpBuf, tmpBuf+ii);
                    j = j - ii-i;
                    outCount = outCount - ii-i;
                    //j--;
                    //printf("%s\n",outBuf);
                    //printf("END: i=%ld ; j=%ld\n", i, j);
                }
                ii = i;
                //printf("i")
            }
        }
    }
    return 0;
}

int processing(char *path)
{
    DIR *dir = NULL;
    DIRENT *curPath = NULL;
    STAT curInfo;
    char pathName[PATH_MAX];
    int numFiles = 0;
    TARGS *arg = NULL;

    open_dir(&dir, path);
    if (dir == NULL) {
        return numFiles;	
    }
        
    while ((curPath = readdir(dir)) != NULL /* && ( (*curPath).d_type == DT_REG || (*curPath).d_type == DT_DIR ) */) {

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

                if (curInfo.st_mode & S_IXOTH) { /*printf("PROPYSTIL - %s\n", pathName);*/ continue; }
                
                printf("%lu: %s\n",pthread_self(), pathName);

                if (tCounter == N) {
                    jj = 0;
                    while (pthread_timedjoin_np(tidMas[jj], NULL, &ts) != 0) {
                        if (jj == N-1)  jj = 0;
                        else jj++;
                    }
                    tCounter--;
                }
                
                /* create struct for arguments of thread function */
                arg = malloc(sizeof(TARGS));
                (*arg).i = jj;
                strcpy((*arg).path, pathName);
                
                //printf("SOZDAEM NA - %d  ;  THREAD COUNT - %d\n", jj, tCounter);
                pthread_create(&(tidMas[jj++]), NULL, &threadF, arg);
                tCounter++;
                
            }
        }
        else {
            fprintf(stderr, "%lu: %s: %s: %s\n",pthread_self(), program, strerror(errno), pathName);
        }
    }
    close_dir(&dir, path);
    
    return numFiles;
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

long getBuf(int fd, char *buf, long size, int m_flag) {
    long count;


    if (m_flag) pthread_mutex_lock(&mutex); //if mutex flag
    
    if ((count = read(fd, buf, size)) == -1) {
        fprintf(stderr, "%s: %s\n", program, strerror(errno));
        exit(errno);
    }
    
    if (m_flag)  pthread_mutex_unlock(&mutex); //if mutex flag

    return count;
}

long putBuf(int fd, char *buf, long size) {
    long count;

    pthread_mutex_lock(&mutex);
    if ((count = write(fd, buf, size)) == -1) {
        fprintf(stderr, "%s: %s\n", program, strerror(errno));
        exit(errno);
    }
    pthread_mutex_unlock(&mutex);

    return count;
}


int errOpen(char *path, int flags) {

    int fd;

    if ((fd = open(path, flags)) == -1) {
        fprintf(stderr, "%lu: %s: %s: %s\n", pthread_self(), program, strerror(errno), path);
        pthread_exit("EXIT");
    }

    return fd;
}

int errClose(int fd) {

    if (close(fd) == -1) {
        fprintf(stderr, "%lu: %s: %s\n",pthread_self(),  program, strerror(errno));
        //exit(errno);
    }

    return 0;
}

long fileSize(char *path) {

    STAT f;

    if (stat(path, &f) == -1) {
        fprintf(stderr, "%s: %s: %s\n", program, strerror(errno), path);
        exit(errno);
    }

    return f.st_size;
}


        
