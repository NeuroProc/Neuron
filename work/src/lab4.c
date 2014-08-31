/*
 * lab4.c : table1 = 10; table2 = 0
 *
 *  Created on: 07.04.2014
 *      Author: lili
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>

#include <sys/wait.h>
#include <fcntl.h>

/* defines */
#define BUFLEN 100
#define PROCCOUNT 9

/* types */
typedef void (*sigaction_t) (int, siginfo_t *, void *);

/*prototypes*/
pid_t getNewPid(void);
int readMas(void);
int ready(int);
void sigHandler1(int, siginfo_t *, void *); // Прототип функции, которая обрабатывает сигнал, переданный процессу (1)
void sigHandler2(int, siginfo_t *, void *);
void sigHandler3(int, siginfo_t *, void *);
void sigHandler4(int, siginfo_t *, void *);
void sigHandler5(int, siginfo_t *, void *);
void sigHandler6(int, siginfo_t *, void *);
void sigHandler7(int, siginfo_t *, void *);
void sigHandler8(int, siginfo_t *, void *);
void setSigHandler(int, sigaction_t); // Прототип функции, которая устанавливает обработчик сигнала отдельному процессу
void sigHandlerQ(int, siginfo_t *, void *);
int increment(int);
int getid(void);

/*global*/
static char *progName = NULL;
static int sigCounter = 0;
static pid_t pid_mas[PROCCOUNT];
static const char *tmp_file = "/tmp/temp_lab4";
static const char *tmp_file_f = "/tmp/temp_f_lab4";


int main(int argc, char *argv[]) {
	progName = argv[0];
	int F = 0, i = 0;
	char stmp[BUFLEN];

	pid_mas[0] = getpid();
	printf("Program: %s\n",progName);
	printf("Start proc main: %d\n",getpid());
	if (getNewPid() == 0){
		pid_mas[1] = getpid();
		printf("Create proc #1: %d: %d\n",getpid(),getppid());
		pid_mas[2] = getNewPid();
		if (pid_mas[2] == 0) {
			printf("Create proc #2: %d: %d\n",getpid(),getppid());
		} else {
			if (getNewPid() == 0) {
				pid_mas[3] = getpid();
				printf("Create proc #3: %d: %d\n",getpid(),getppid());
				if (getNewPid() == 0) {
					pid_mas[4] = getpid();
					printf("Create proc #4: %d: %d\n",getpid(),getppid());
					pid_mas[5] = getNewPid();
					if (pid_mas[5] == 0) {
						printf("Create proc #5: %d: %d\n",getpid(),getppid());
					} else {
						if (getNewPid() == 0) {
							pid_mas[6] = getpid();
							printf("Create proc #6: %d: %d\n",getpid(),getppid());
							if (getNewPid() == 0) {
								pid_mas[7] = getpid();
								printf("Create proc #7: %d: %d\n",getpid(),getppid());
								if (getNewPid() == 0){
									pid_mas[8] = getpid();
									printf("Create proc #8: %d: %d\n",getpid(),getppid());
									
                                    memset(stmp, 0, BUFLEN);
									for (i = 0; i < PROCCOUNT; i++) {
										sprintf((stmp + strlen(stmp)),"%d ",pid_mas[i]);
                                        //printf("FOR: %d: %s\n",i,stmp);
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

								}
							}

						}
					}
				}

			}
		}
	}

	readMas();
    /*
    printf("\n");
	for (i = 0; i < 9; i++) {
			printf("%d ",pid_mas[i]);
		}
	printf("\n");
    */
	// Create group of processes (3), (4)
	setpgid(pid_mas[3], pid_mas[3]);
	setpgid(pid_mas[4], pid_mas[3]);


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

	i = getpid();
	if (i == pid_mas[1])
		setSigHandler(SIGUSR2, &sigHandler1);
	else if (i == pid_mas[2])
		setSigHandler(SIGUSR1, &sigHandler2);
	else if (i == pid_mas[3])
		setSigHandler(SIGUSR2, &sigHandler3);
	else if (i == pid_mas[4])
		setSigHandler(SIGUSR2, &sigHandler4);
	else if (i == pid_mas[5])
		setSigHandler(SIGUSR1, &sigHandler5);
	else if (i == pid_mas[6])
		setSigHandler(SIGUSR1, &sigHandler6);
	else if (i == pid_mas[7])
		setSigHandler(SIGUSR1, &sigHandler7);
	else if (i == pid_mas[8])
		setSigHandler(SIGUSR2, &sigHandler8);

	setSigHandler(SIGTERM, &sigHandlerQ);

    //printf("%d: INCREMENT\n",getpid());
	increment(F);

    //sleep(10);
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

	if (getpid() == pid_mas[1]) {
		printf("START!!!\n");
		kill(pid_mas[2], SIGUSR1);
	}

    if (pid_mas[0] != getpid()) {
        while (1) {
            pause();
        }
    }

	waitpid(pid_mas[1], NULL, 0);

	/* wait while all of the processes not end */
	/*
	errno = 0;
	while (errno != ECHILD) {
		wait(NULL);
	}
	*/
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
	int fd = 0, count = 0;
	fd_set fdset;
	char buf[BUFLEN];

	while (access(tmp_file, F_OK) == -1) {

	}

	if ((fd = open(tmp_file, O_RDONLY)) == -1) {
		fprintf(stderr, "%d: %s: %s: %s\n",getpid(), progName, strerror(errno), tmp_file);
		exit(errno);
	}

	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);

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
    
    lseek(fd, getid(), SEEK_SET);
    if (write(fd, "a", 1) == -1) {
		fprintf(stderr, "%d: %s: %s: %s", getpid(), progName, strerror(errno), tmp_file);
		return -1;
	}

	return 0;
}

int getid(void) {
    int pid = getpid();

    for (int i = 0; i < PROCCOUNT; i++)
        if (pid == pid_mas[i])
            return i;

    return -1;
}

void sigHandler1 (int sig, siginfo_t *info, void *context) {

	printf("PID %d: PPID %d: GET %s: from %d: time: %ld\n",
			getpid(), getppid(), strsignal(sig), (*info).si_pid, time(NULL));

	sigCounter++;
    printf("SIGCOUNTER: %d\n",sigCounter);
    
    if (sigCounter == 101) {
		kill(pid_mas[2], SIGTERM);
		waitpid(pid_mas[2], NULL, 0);
		raise(SIGTERM);
	}

    printf("PID %d: PPID %d: PUT %s: to %d: time: %ld\n",
           getpid(), getppid(), strsignal(SIGUSR1), pid_mas[2], time(NULL));
	if (kill(pid_mas[2], SIGUSR1) == -1) {
		fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
		exit(errno);
	}
}

void sigHandler2 (int sig, siginfo_t *info, void *context) {

	printf("PID %d: PPID %d: GET %s: from %d: time: %ld\n",
			getpid(), getppid(), strsignal(sig), (*info).si_pid, time(NULL));

	sigCounter++;
    
    printf("PID %d: PPID %d: PUT %s: to %d: time: %ld\n",
           getpid(), getppid(), strsignal(SIGUSR2), -pid_mas[3], time(NULL));
	if (kill(-pid_mas[3], SIGUSR2) == -1) {
		fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
		exit(errno);
	}
}

void sigHandler3 (int sig, siginfo_t *info, void *context) {

	printf("PID %d: PPID %d: GET %s: from %d: time: %ld\n",
			getpid(), getppid(), strsignal(sig), (*info).si_pid, time(NULL));

	sigCounter++;

    printf("PID %d: PPID %d: PUT %s: to %d: time: %ld\n",
           getpid(), getppid(), strsignal(SIGUSR1), pid_mas[6], time(NULL));
    if (kill(pid_mas[6], SIGUSR1) == -1) {
        fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
        exit(errno);
    }
}

void sigHandler4 (int sig, siginfo_t *info, void *context) {

	printf("PID %d: PPID %d: GET %s: from %d: time: %ld\n",
			getpid(), getppid(), strsignal(sig), (*info).si_pid, time(NULL));

	sigCounter++;

    printf("PID %d: PPID %d: PUT %s: to %d: time: %ld\n",
           getpid(), getppid(), strsignal(SIGUSR1), pid_mas[5], time(NULL));
    if (kill(pid_mas[5], SIGUSR1) == -1) {
        fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
        exit(errno);
    }
}

void sigHandler5 (int sig, siginfo_t *info, void *context) {

	printf("PID %d: PPID %d: GET %s: from %d: time: %ld\n",
			getpid(), getppid(), strsignal(sig), (*info).si_pid, time(NULL));

	sigCounter++;
}

void sigHandler6 (int sig, siginfo_t *info, void *context) {

	printf("PID %d: PPID %d: GET %s: from %d: time: %ld\n",
			getpid(), getppid(), strsignal(sig), (*info).si_pid, time(NULL));

	sigCounter++;

    printf("PID %d: PPID %d: PUT %s: to %d: time: %ld\n",
           getpid(), getppid(), strsignal(SIGUSR1), pid_mas[7], time(NULL));
	if (kill(pid_mas[7], SIGUSR1) == -1) {
		fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
		exit(errno);
	}
}

void sigHandler7 (int sig, siginfo_t *info, void *context) {

	printf("PID %d: PPID %d: GET %s: from %d: time: %ld\n",
			getpid(), getppid(), strsignal(sig), (*info).si_pid, time(NULL));

	sigCounter++;

    printf("PID %d: PPID %d: PUT %s: to %d: time: %ld\n",
           getpid(), getppid(), strsignal(SIGUSR2), pid_mas[8], time(NULL));
	if (kill(pid_mas[8], SIGUSR2) == -1) {
		fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
		exit(errno);
	}
}

void sigHandler8 (int sig, siginfo_t *info, void *context) {

	printf("PID %d: PPID %d: GET %s: from %d: time: %ld\n",
			getpid(), getppid(), strsignal(sig), (*info).si_pid, time(NULL));

	sigCounter++;

    printf("PID %d: PPID %d: PUT %s: to %d: time: %ld\n",
           getpid(), getppid(), strsignal(SIGUSR2), pid_mas[1], time(NULL));
	if (kill(pid_mas[1], SIGUSR2) == -1) {
		fprintf(stderr, "%d: %s: %s\n", getpid(), progName, strerror(errno));
		exit(errno);
	}
}

void sigHandlerQ (int sig, siginfo_t *info, void *context) {
	pid_t a = getpid();

	if (a == pid_mas[2]) {
		kill(-pid_mas[3], SIGTERM);
		waitpid(pid_mas[3], NULL, 0);
		waitpid(pid_mas[4], NULL, 0);
	} else if (a == pid_mas[3]) {
		kill(pid_mas[6], SIGTERM);
		waitpid(pid_mas[6], NULL, 0);
	} else if (a == pid_mas[4]) {
		kill(pid_mas[5], SIGTERM);
		waitpid(pid_mas[5], NULL, 0);
	} else if (a == pid_mas[6]) {
		kill(pid_mas[7], SIGTERM);
		waitpid(pid_mas[7], NULL, 0);
	} else if (a == pid_mas[7]) {
		kill(pid_mas[8], SIGTERM);
		waitpid(pid_mas[8], NULL, 0);
	}

	// Вывод сообщения о завершении работы процесса
	//printMessage("finished");
	printf("%d: FINISHED: time: %ld mksec\n", getpid(), time(NULL));

	exit(0);
}
