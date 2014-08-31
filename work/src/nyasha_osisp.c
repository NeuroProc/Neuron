	

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <errno.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <fcntl.h>
     
    #define SIZE 256
    #define NONE_REDIRECTION 0
    #define IN_REDIRECTION 1
    #define OUT_REDIRECTION 2
    #define APPEND_REDIRECTION 3
     
    char* progName;
     
    pid_t getNewPid (); //возвращает идентификатор созданного процесса и контролирующая ошибку создания процесса
    void runCommand (char*); //выполняет заданную команду, используя вызов exec(), и контролирующая ошибку выполнения команды
    void redirect (char*, int, int, int); //перенаправление ввода / вывода
     
    int main (int countOfArgs, char* args[]) {
            char* command = (char*) malloc(SIZE);
            pid_t newPid;
     
            progName = args[0];
     
            printf("%s> ", args[0]);
            gets(command);
            while (strcmp(command, "exit") != 0) {
     
                    newPid = getNewPid();
     
                    if (newPid == 0)
                            runCommand(command);
                    else {
                            int childStatVal;
                            pid_t childPid;
                           
                            // Приостановка выполнения родительского процесса
                            childPid = waitpid(newPid, &childStatVal, 0);
                           
                            if (!WIFEXITED(childStatVal)) { //проверка корректности завершения процесса сына
                                    printf("Child terminated abnormally\n");
                            }
     
                            printf("%s> ", args[0]);
                            gets(command);
                    }
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
     
    void runCommand (char* command) {
            char **cmdPart = (char **) malloc(SIZE);
            char *string, *inFilePath, *outFilePath;
            int i = 0, result = 0;
            int redirectionKind = NONE_REDIRECTION; // Флаг перенаправления
     
            cmdPart[i] = strtok(command, " "); //strtok разделяет строку на лексемы
            i++;
            while ((string = strtok(NULL, " ")) != NULL) {
                    // перенаправление ввода
                    if (strcmp(string, "<") == 0) {
                            redirectionKind = IN_REDIRECTION;
                            continue;
                    }
                    // перенаправление вывода
                    else if (strcmp(string, ">") == 0) {
                            redirectionKind = OUT_REDIRECTION;
                            continue;
                    }
                    // дозапись файла
                    else if (strcmp(string, ">>") == 0) {
                            redirectionKind = APPEND_REDIRECTION;
                            continue;
                    }
     
                    // Если перенаправления вывода еще не было
                    if (redirectionKind == NONE_REDIRECTION) {
                            cmdPart[i] = string;
                            i++;
                    }
                    // Записать путь к файлу, если было перенаправление ввода
                    else if (redirectionKind == IN_REDIRECTION) {
                            inFilePath = string;
                            continue;
                    }
                    // Записать путь к файлу, если было перенаправление вывода / дозаписи /
                    else if ((redirectionKind == OUT_REDIRECTION) ||
                            (redirectionKind == APPEND_REDIRECTION)) {
                            outFilePath = string;
                            continue;
                    }
            }
            cmdPart[i] = (char*) 0;
     
            if (redirectionKind != NONE_REDIRECTION) {
                    if (redirectionKind == IN_REDIRECTION) {
                            redirect(inFilePath, O_RDONLY, 0660, 0);
                    }
                    else if (redirectionKind == OUT_REDIRECTION) {
                            redirect(outFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0660, 1);
                    }
                    // Получение дескриптора для файла вывода, если было перенаправление для дозаписи файла
                    else if (redirectionKind == APPEND_REDIRECTION) {
                            redirect(outFilePath, O_APPEND | O_WRONLY | O_CREAT, 0660, 1);
                    }
            }
     
            result = execvp(cmdPart[0], cmdPart); //execvp запускает на выполнение команду
            if (result == -1) {
                    fprintf(stderr, "%s : %s\n", progName, strerror(errno));
                    exit(errno);
            }
    }
     
    int openFile (char* path, int mode, int rights) {
            int fileD;
     
            if ((fileD = open(path, mode, rights)) != -1)
                    return fileD;
            else {
                    fprintf(stderr, "%s : %s : %s\n", progName, strerror(errno), path);
                    exit(errno);
            }
    }
     
    void redirect (char* filePath, int mode, int rights, int newFD) {
            int fileD = openFile(filePath, mode, rights);
            int fileDDup = dup2(fileD, newFD);
            close(fileD);
    }

