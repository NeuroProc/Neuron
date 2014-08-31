#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
 
#include <sys/wait.h>
#include <fcntl.h>
 
/* defines */
#define BUFLEN 100
#define PROCCOUNT 9
#define NUMBER_OF_ITERATION 101
 
/* types */
typedef void (*sigaction_t) (int, siginfo_t *, void *);
 
/*prototypes*/
pid_t getNewPid(void);
int readMas(void);
int ready(int);
void sigHandler1(int, siginfo_t *, void *); // обработка сигнала, переданного процессу (1)
void sigHandler2(int, siginfo_t *, void *);
void sigHandler3(int, siginfo_t *, void *);
void sigHandler4(int, siginfo_t *, void *);
void sigHandler5(int, siginfo_t *, void *);
void sigHandler6(int, siginfo_t *, void *);
void sigHandler7(int, siginfo_t *, void *);
void sigHandler8(int, siginfo_t *, void *);
void setSigHandler(int, sigaction_t); //  обработчик сигнала отдельному процессу
void sigHandlerQ(int, siginfo_t *, void *);
int increment(int);
int getid(void);                //uznat nomer v massive po PIDy
void printGet (char *, int); // information about getting
void printSend (char *, int); // information about sending
void printMessage (char*, int); // information about start or finish
 
/*global*/
static char *progName = NULL;
static int sigCounter = 0;
static pid_t pid_mas[PROCCOUNT];
static const char *tmp_file = "/tmp/temp_lab4";
static const char *tmp_file_f = "/tmp/temp_f_lab4";
static int processNumber = 0; // Номер процесса
static pid_t PPID = 0;
static pid_t PID = 0;
int S;
 
 
int main(int argc, char *argv[]) {
        progName = argv[0];
        int F = 0, i = 0;
        char stmp[BUFLEN];
        int p[2];
       
        pipe(p);        //sozdali neimenovannyi kanal, k kotoromy imejut dostup vse deti glavnogo processa
 
        pid_mas[0] = getpid();
        if (getNewPid() == 0){          //sozdali #1
                pid_mas[1] = getpid();
                processNumber = 1;
                printMessage("started", 0);
                if (getNewPid() == 0) {         //sozdali #2
                        pid_mas[2] = getpid();
                        processNumber = 2;
                        printMessage("started", 0);
                        if (getNewPid() == 0) {         //sozdali #3
                                pid_mas[3] = getpid();
                                processNumber = 3;
                                printMessage("started", 0);
                                pid_mas[4] = getNewPid();
                                if (pid_mas[4] == 0) {          //sozdali #4, ne if (getNewPid() == 0) { dlya togo, chtoby ne proebat pid,
                                        processNumber = 4;              //chtoby v konce 7 process znal ve pidy krome 8
                                        printMessage("started", 0);
                                        if (getNewPid() == 0){
                                                pid_mas[8] = getpid();
                                                processNumber = 8;
                                                printMessage("started", 0);
                                                memset(stmp,0,BUFLEN);  //obnulyaem stmp
                                                sprintf(stmp,"%d",pid_mas[8]);  //preobrazuem k stroke
                                                if (write(p[1], stmp, BUFLEN) == -1) {          //zapisyvaem pid8 v kanal
                            fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
                            return -1;
                        }
                                                close(p[1]);
                                        }
                                } else {
                                        pid_mas[5] = getNewPid();
                                        if (pid_mas[5] == 0) {
                                                processNumber = 5;
                                                printMessage("started", 0);
                                        } else {
                                                if (getNewPid() == 0) {
                                                        pid_mas[6] = getpid();
                                                        processNumber = 6;
                                                        printMessage("started", 0);
                                                        if (getNewPid() == 0) {
                                                                pid_mas[7] = getpid();
                                                                processNumber = 7;
                                                                printMessage("started", 0);
                                                                //////////////
                                                                memset(stmp, 0, BUFLEN);
                                                                if (read(p[0], stmp, BUFLEN) == -1) {   //schityvaem 8 pid iz kanala
                                                                        fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
                                    return -1;
                                }
                                                                close(p[0]);
                                                                sscanf(stmp,"%d",&pid_mas[8]);  //preobrazuem k chislu
 
                                                                memset(stmp, 0, BUFLEN);
                                                                for (i = 0; i < PROCCOUNT; i++) {
                                                                        sprintf((stmp + strlen(stmp)),"%d ",pid_mas[i]);
                                   
                                }
 
                                if ((F = open(tmp_file, O_RDWR | O_CREAT, 0777)) == -1) {
                                                                        fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
                                                                        return -1;
                                                                }
                                if (write(F, stmp, strlen(stmp)) == -1) {
                                    fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
                                    return -1;
                                }
 
                                                                if (close(F) == -1) {
                                                                        fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
                                                                        return -1;
                                                                }
                                                                ////////////////
                                                        }
                                                }
                                        }
                                }
                        }
                }                              
        }
        sleep(1);
 
        PPID = getppid();
        PID = getpid();
 
        readMas();
 
        // Create group of processes (2), (3), (4)
        setpgid(pid_mas[2], pid_mas[2]);
        setpgid(pid_mas[3], pid_mas[2]);
        setpgid(pid_mas[4], pid_mas[2]);
        // Create group of processes (5), (6)
        setpgid(pid_mas[5], pid_mas[5]);
        setpgid(pid_mas[6], pid_mas[5]);
 
        //sozdanie faila flagov, chtoby ulovit moment, kogda vse processy ustanovyat obrabotchiki signalov
        if (getpid() == pid_mas[0]) {
                if ((F = open(tmp_file_f, O_RDWR | O_CREAT, 0777)) == -1) {
                        fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
                        return -1;
                }
        write(F, "111111111", PROCCOUNT * sizeof(char));
        } else {
                while (access(tmp_file_f, F_OK) == -1) {
 
                }
 
                if ((F = open(tmp_file_f, O_RDWR)) == -1) {
                        fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
                        return -1;
                }
        }
       
        //ustanovka obrabotchikov signalov
        i = getpid();
        if (i == pid_mas[1])
                setSigHandler(SIGUSR2, &sigHandler1);
        else if (i == pid_mas[2])
                setSigHandler(SIGUSR1, &sigHandler2);
        else if (i == pid_mas[3])
                setSigHandler(SIGUSR1, &sigHandler3);
        else if (i == pid_mas[4])
                setSigHandler(SIGUSR1, &sigHandler4);
        else if (i == pid_mas[5])
                setSigHandler(SIGUSR2, &sigHandler5);
        else if (i == pid_mas[6])
                setSigHandler(SIGUSR2, &sigHandler6);
        else if (i == pid_mas[7])
                setSigHandler(SIGUSR1, &sigHandler7);
        else if (i == pid_mas[8])
                setSigHandler(SIGUSR1, &sigHandler8);
 
        setSigHandler(SIGTERM, &sigHandlerQ);
 
        increment(F); //ustanovka flaga sootvetstvujuschego processa v faile, kak tolko ustanovlen obrabotchik
 
        //poka vse obrabotchiki ne ustanovyatsya
        while(strcmp(stmp,"aaaaaaaaa") != 0) {
                memset(stmp, 0, BUFLEN);
                while (ready(F) == -1) {
 
                }
                lseek(F, 0, SEEK_SET);
                if (read(F, stmp, PROCCOUNT) == -1) {
                        fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file_f);
                        exit(errno);
                }
        stmp[PROCCOUNT] = '\0';
        }
 
        if (close(F) == -1) {
                fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file_f);
                exit(errno);
        }
 
        //nachinaem krugovorot posylki signalov
        if (getpid() == pid_mas[1]) {
                printf("\n");
                printSend("send", 1);
                kill(pid_mas[2], SIGUSR1);
        }
 
    if (pid_mas[0] != getpid()) {
        while (1) {
            pause();
        }
    }
 
        waitpid(pid_mas[1], NULL, 0);
 
        if (pid_mas[0] == getpid()) {
                unlink(tmp_file);
                unlink(tmp_file_f);
        }
 
        return 0;
}
 
pid_t getNewPid () {
        pid_t newPid;
        if ((newPid = fork()) != -1) {
                return newPid;
        }
        else {
                fprintf(stderr, "%s : %s\n", progName, strerror(errno));
                exit(errno);
        }
}
 
int readMas(void) {
        int fd = 0;
        char buf[BUFLEN];
 
        while (access(tmp_file, F_OK) == -1) {
 
        }
 
        if ((fd = open(tmp_file, O_RDONLY)) == -1) {
                fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
                exit(errno);
        }
 
        while (ready(fd) == -1) {
 
        }
        read(fd, buf, BUFLEN);
        close(fd);
 
        sscanf(buf,"%d %d %d %d %d %d %d %d %d",&(pid_mas[0]), &(pid_mas[1]),&(pid_mas[2]),
                        &(pid_mas[3]),&(pid_mas[4]),&(pid_mas[5]),&(pid_mas[6]),&(pid_mas[7]),&(pid_mas[8]));
 
        return 0;
}
 
void setSigHandler (int sig, sigaction_t handler) {
        struct sigaction sa;
 
        memset(&sa, 0, sizeof(struct sigaction));
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = handler;
        if (sigaction(sig, &sa, NULL) == -1) {
                fprintf(stderr, "%s: %s\n", progName, strerror(errno));
                exit(errno);
        }
}
 
int ready(int fd) {
 
        fd_set fdset;
 
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
 
        switch (select(fd + 1, &fdset, NULL, NULL, NULL)) {
                case -1: {
                        fprintf(stderr, "%d: %s: %s\n",getpid(), progName, strerror(errno));
                        return -1;
                }
                case 0: return -1;
                default: {
                        if (FD_ISSET(fd, &fdset))
                                return 1;
                        else
                                return -1;
                }
        }
}
 
int increment(int fd) {
 
        while(ready(fd) == -1) {
        sleep(1);
        }
 
    lseek(fd, getid(), SEEK_SET);  //(fd, smeschenie, otkuda)
    if (write(fd, "a", 1) == -1) {
                fprintf(stderr, "%d: %s: %s: %s", getpid(), progName, strerror(errno), tmp_file);
                return -1;
        }
 
        return 0;
}
 
int getid(void) {
    int pid = getpid();
    int i = 0;
 
    for (i = 0; i < PROCCOUNT; i++) {
        if (pid == pid_mas[i])
            return i;
    }
 
    return -1;
}
 
void sigHandler1 (int sig, siginfo_t *info, void *context) {
 
        printGet("get", 2);
 
        sigCounter++;
 
    if (sigCounter == NUMBER_OF_ITERATION) {
        printf("\n");
                //kill(pid_mas[2], SIGTERM);
                //waitpid(pid_mas[2], NULL, 0);
                raise(SIGTERM);
        }
    printSend("send", 1);
 
        if (kill(-pid_mas[2], SIGUSR1) == -1) {
                fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
                exit(errno);
        }
}
 
void sigHandler2 (int sig, siginfo_t *info, void *context) {
 
        printGet("get", 1);
 
        sigCounter++;
 
        printSend("send", 2);
 
        if (kill(-pid_mas[5], SIGUSR2) == -1) {
                fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
                exit(errno);
        }
}
 
void sigHandler3 (int sig, siginfo_t *info, void *context) {
 
        printGet("get", 1);
 
        sigCounter++;
 
}
 
void sigHandler4 (int sig, siginfo_t *info, void *context) {
 
        printGet("get", 1);
 
        sigCounter++;
 
}
 
void sigHandler5 (int sig, siginfo_t *info, void *context) {
 
        printGet("get", 2);
 
        sigCounter++;
}
 
void sigHandler6 (int sig, siginfo_t *info, void *context) {
 
        printGet("get", 2);
 
        sigCounter++;
 
        printSend("send", 1);
        if (kill(pid_mas[7], SIGUSR1) == -1) {
                fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
                exit(errno);
        }
}
 
void sigHandler7 (int sig, siginfo_t *info, void *context) {
 
        printGet("get", 1);
 
        sigCounter++;
 
        printSend("send", 1);
 
        if (kill(pid_mas[8], SIGUSR1) == -1) {
                fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
                exit(errno);
        }
}
 
void sigHandler8 (int sig, siginfo_t *info, void *context) {
 
        printGet("get", 1);
 
        sigCounter++;
 
        printSend("send", 2);
        if (kill(pid_mas[1], SIGUSR2) == -1) {
                fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
                exit(errno);
        }
}
 
void sigHandlerQ (int sig, siginfo_t *info, void *context) {
        pid_t a = getpid();
 
        if (a == pid_mas[1]) {
                kill(pid_mas[2], SIGTERM);
                waitpid(pid_mas[2], NULL, 0);
                printMessage("finished", 1);
        } else if (a == pid_mas[2]) {
                kill(pid_mas[3], SIGTERM);
                printMessage("finished", 2);
        } else if (a == pid_mas[3]) {
                kill(pid_mas[4], SIGTERM);
                kill(pid_mas[5], SIGTERM);
                kill(pid_mas[6], SIGTERM);
                waitpid(pid_mas[4], NULL, 0);
                waitpid(pid_mas[5], NULL, 0);
                waitpid(pid_mas[6], NULL, 0);
                printMessage("finished", 3);
        } else if (a == pid_mas[4]) {
                kill(pid_mas[8], SIGTERM);
                waitpid(pid_mas[8], NULL, 0);
                printMessage("finished", 3);
        } else if (a == pid_mas[5]) {
                printMessage("finished", 3); //3
        } else if (a == pid_mas[6]) {
                kill(pid_mas[7], SIGTERM);
                waitpid(pid_mas[7], NULL, 0);
                printMessage("finished", 1);
        } else if (a == pid_mas[7]) {
                printMessage("finished", 1);
        } else if (a == pid_mas[8]) {
                printMessage("finished", 2);
        }
        //printMessage("finished",1);
 
        exit(0);
}
 
void printGet (char* action, int flag) {
        struct timeval tml;
    gettimeofday(&tml,NULL);
 
        // Вывод ID текущего процесса, ID родительского процесса, времени (мксек)
    if (flag == 1) {
        printf("%d %d %d %s %s %ld\n",
                        processNumber, PID, PPID, action, "SIGUSR1", tml.tv_usec);
    } else {
        printf("%d %d %d %s %s %ld\n",
                        processNumber, PID, PPID, action, "SIGUSR2", tml.tv_usec);
    }
}
 
// Функция, выводящая информацию о процессе и о полученном/переданном сигнале
void printSend (char* action, int flag) {
        struct timeval tml;
        gettimeofday(&tml,NULL);
 
        // Вывод ID текущего процесса, ID родительского процесса, времени (мксек)
        if (flag == 1) {
                printf("%d %d %d %s %s %ld\n",
                                processNumber, PID, PPID, action, "SIGUSR1", tml.tv_usec);
        } else {
                printf("%d %d %d %s %s %ld\n",
                                processNumber, PID, PPID, action, "SIGUSR2", tml.tv_usec);
        }
}
void printMessage (char* message, int flag) {
        struct timeval tml;
        gettimeofday(&tml,NULL);
 
        if (strcmp(message, "started") == 0) {
                printf("%d %d %d %s %ld\n",
                        processNumber, getpid(), getppid(), message, tml.tv_usec);
        }
        else if (strcmp(message, "finished") == 0) {
                switch (flag) {
                        case 1: { printf("%d %d %d %s after %d signals SIGUSR1 and 0 signals SIGUSR2 %ld\n",
                                                        processNumber, PID, PPID, message, sigCounter, tml.tv_usec); break;
                        }
                        case 2: { printf("%d %d %d %s after 0 signals SIGUSR1 and %d signals SIGUSR2 %ld\n",
                                                        processNumber, PID, PPID, message, sigCounter, tml.tv_usec); break;
                        }
                        case 3: { printf("%d %d %d %s after 0 signals SIGUSR1 and 0 signals SIGUSR2 %ld\n",
                                                        processNumber, PID, PPID, message, tml.tv_usec); break;
                        }
                        default: break;
                }
        }
}
