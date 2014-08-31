	

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
     
    pid_t getNewPid (); //���������� ������������� ���������� �������� � �������������� ������ �������� ��������
    void runCommand (char*); //��������� �������� �������, ��������� ����� exec(), � �������������� ������ ���������� �������
    void redirect (char*, int, int, int); //��������������� ����� / ������
     
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
                           
                            // ������������ ���������� ������������� ��������
                            childPid = waitpid(newPid, &childStatVal, 0);
                           
                            if (!WIFEXITED(childStatVal)) { //�������� ������������ ���������� �������� ����
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
            int redirectionKind = NONE_REDIRECTION; // ���� ���������������
     
            cmdPart[i] = strtok(command, " "); //strtok ��������� ������ �� �������
            i++;
            while ((string = strtok(NULL, " ")) != NULL) {
                    // ��������������� �����
                    if (strcmp(string, "<") == 0) {
                            redirectionKind = IN_REDIRECTION;
                            continue;
                    }
                    // ��������������� ������
                    else if (strcmp(string, ">") == 0) {
                            redirectionKind = OUT_REDIRECTION;
                            continue;
                    }
                    // �������� �����
                    else if (strcmp(string, ">>") == 0) {
                            redirectionKind = APPEND_REDIRECTION;
                            continue;
                    }
     
                    // ���� ��������������� ������ ��� �� ����
                    if (redirectionKind == NONE_REDIRECTION) {
                            cmdPart[i] = string;
                            i++;
                    }
                    // �������� ���� � �����, ���� ���� ��������������� �����
                    else if (redirectionKind == IN_REDIRECTION) {
                            inFilePath = string;
                            continue;
                    }
                    // �������� ���� � �����, ���� ���� ��������������� ������ / �������� /
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
                    // ��������� ����������� ��� ����� ������, ���� ���� ��������������� ��� �������� �����
                    else if (redirectionKind == APPEND_REDIRECTION) {
                            redirect(outFilePath, O_APPEND | O_WRONLY | O_CREAT, 0660, 1);
                    }
            }
     
            result = execvp(cmdPart[0], cmdPart); //execvp ��������� �� ���������� �������
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

