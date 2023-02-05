#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1;
#define FALSE 0;

/**
 * A linked list data structure to store our processes and navigate through them.
 * PID: Process ID 
 * command: Name of program 
 * isRunning:  True or false if its currently running or not
 * next: reference to next node;
 */
typedef struct node {
	pid_t pid;
	char* command;
    int isRunning;
	struct node *next;
} node;

node* head = NULL;

/**
 * Takes in PID and command name, adds to end of process linked list.
 */ 
void addNode(pid_t PID, char* command) {

    node* process = (struct node*)malloc(sizeof(struct node));
    process->pid = PID;
    process->command = command;
    process->isRunning = TRUE;
    process->next = NULL;

    if(head == NULL) {
        head = process;
    } else {
        node* iteratorNode = head;
        while(iteratorNode->next != NULL) {
            iteratorNode = iteratorNode->next;
        }
        iteratorNode->next = process;
    }
}

/**
 * Function to loop through our linked list and print all PID and command names.
 */ 
void printList() {

    node* iteratorNode = head;

    while(iteratorNode != NULL) {
        printf("%d: %s\n", iteratorNode->pid, iteratorNode->command);
        iteratorNode = iteratorNode->next;
    }
}

/**
 * Given an input PID, we will remove the corresponding node in our linked list,
 * and return an error if the node does not exist.
 */

void removeNode(pid_t PID) {

    node* iteratorNode = head;

    if(iteratorNode->pid == PID) {
        head = head->next;
        free(iteratorNode);
    } else {
        iteratorNode = iteratorNode->next;
        node* prevTemp = head;
        while((iteratorNode != NULL) && (iteratorNode->pid != PID)) {
            iteratorNode = iteratorNode->next;
            prevTemp = prevTemp->next;
        }
        if(iteratorNode == NULL) {
            printf("Invalid PID\n");
            return;
        }
        prevTemp->next = iteratorNode->next;
        free(iteratorNode);
    }
    if(iteratorNode == NULL) {
        printf("PID does not exist\n");
    }
}

/**
 * Given a PID and a status, update the specified node's isRunning field.
 * If the process linked list is empty, prints out an error message.
 */
void setRunning(pid_t PID, int status) {

    if(head == NULL) {
        printf("No background processes\n");
    } else {
        node* iteratorNode = head;
        while(iteratorNode->pid != PID) {
            iteratorNode = iteratorNode->next;
        }
        if(iteratorNode == NULL) {
            printf("No such process with PID %d\n", PID);
            return;
        } else {
            iteratorNode->isRunning = status;
        }
    }
}

/**
 * Function to determine whether or not a process exists in our linked list
 */
int nodeExists(pid_t PID) {
    node* iteratorNode = head;

    if(head ==  NULL) {
        return 0;
    } else {
        while(iteratorNode != NULL) {
            if(iteratorNode->pid == PID) {
                return 1;
            }
            iteratorNode = iteratorNode->next;
        }
    }
    return 0;
}

/**
 * Given a PID, return the corresponding  node*
 */
node* getNode(pid_t PID) {
    node* iteratorNode = head;

    if(head == NULL) {
        return NULL;
    } else {
        while(iteratorNode != NULL) {
            if(iteratorNode->pid == PID) {
                printf("Returning Node with PID = %d\n", PID);
                return iteratorNode;
            }
            iteratorNode = iteratorNode->next;
        }
    }
    return NULL;
}

/**
 * Function that counts total number of nodes present in LL.
 * Returns int.
 */ 
int bgcount() {

    node* iteratorNode = head;

    int counter = 0;
    while(iteratorNode != NULL) {
        counter++;
        iteratorNode = iteratorNode->next;
    }
    printf("Total background jobs: %d\n", counter);
    return counter;
}

char* getPath(pid_t PID) {
    node* myNode = getNode(PID);

    return myNode;
}

void readStatFile(char* procPath, char** data) {

    printf("stat path is %s\n", procPath);
    FILE* statFile = fopen(procPath, "r");
    char fileContents[1024];

    if(statFile != NULL) {
        int iterator = 0;
        int dataSize = sizeof(fileContents)-1;
        while(fgets(fileContents, dataSize, statFile) != NULL) {
            char* token = strtok(fileContents, " ");
            data[iterator] = token;
            while(token != NULL) {
                data[iterator] = token;
                token = strtok(NULL, " ");
                iterator++;
            }
        }
        fclose(statFile);
    } else if(statFile == NULL) {
        printf("Could not read stat file \n");
    }
    for(int i = 0; i<20; i++) {
        printf("%s", data[i]);
    }
    printf("\n");

}

void readStatusFile(char* procPath, char** data) {

    printf("status path is %s\n", procPath);
    FILE* statusFile = fopen(procPath, "r");
    char fileContents[1024];

    if(statusFile != NULL) {
        int iterator = 0;
        int dataSize = sizeof(fileContents)-1;
        while(fgets(fileContents, dataSize, statusFile) != NULL) {
            char* token = strtok(fileContents, " ");
            data[iterator] = token;
            while(token != NULL) {
                data[iterator] = token;
                token = strtok(NULL, " ");
                iterator++;
            }
        }
        fclose(statusFile);
    }  else if(statusFile == NULL) {
        printf("Could not read status file\n");
    }
    for(int i = 0; i<20; i++) {
        printf("%s", data[i]);
    }
    printf("\n");
    
}

void pstat(pid_t PID) {
    if(nodeExists(PID)) {
        node* processNode = getNode(PID);
        char* status[200];

        printf("here\n");
        printf("PID = ok\n");
        char* stat[200];
        sprintf(stat, "/proc/%d/stat", PID);
        sprintf(status, "/proc/%d/status", PID);

        char* fileContents[200];
        FILE* statFile = fopen(stat, "r");
        FILE* statusFile = fopen(status, "r");
        char statData[128];
        char statusData[128];

        readStatFile(statFile, statData);
        readStatusFile(statusFile, statusData);
        
    } else {
        printf("Invalid PID\n");
    }
}

/**
 * Get user input.
 * 
 * How it works:
 * Given an input, program will tokenize string
 * and depending on input (bg, bglist, ...etc.),
 * it will work with the linked list to perform
 * the given actions.
 * 
 * Supported commands are:
 * bg, bglist, bgkill, bgstart, bgstop, pstat
 * 
 * Custom commands:
 * exit
 * 
 * If the user enters "exit" as a command, PMan will
 * terminate and allow the user to return to their
 * normal terminal interface.
 */
void inputHandler() {

    char* myCommand = readline("PMan: >");
    char* token = strtok(myCommand, " ");
    char* commands[10];
 
    if(strcmp(token, "bg") == 0) {
        token = strtok(NULL, " ");
        char* arguments = token;    //Saving a copy of token
        if(token == NULL) {
            printf("No program given\n");
        } else {
            pid_t PID = fork();
            if(PID > 0) {   //parent
                addNode(PID, token);
                printf("Started new process with PID: %d\n", PID);
                sleep(1);
            } else if(PID == 0) {   //child
                //Iterate through remaining commands, storing them into commands[] to pass to execvp()
                int i = 0;
                commands[i] = arguments;
                i++;
                arguments = strtok(NULL, " ");
                while(arguments != NULL) {
                    commands[i] = arguments;
                    arguments = strtok(NULL, " ");
                    i++;
                }
                commands[i] = NULL;
                execvp(token, commands);
                printf("execvp() failed\n");
                exit(1);
            } else {
                printf("Fork error\n");
            }
        }

    }else if(strcmp(token, "bglist") == 0) {
        printList();
        bgcount();

    }else if(strcmp(token, "bgkill") == 0) {
        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Invalid PID given\n");
        } else {
            pid_t removalPID = atoi(token);
            removeNode(removalPID);
        }
    

    }else if(strcmp(token, "bgstop") == 0) {
        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Invalid PID given\n");
        } else {
            pid_t PID = atoi(token);
            setRunning(PID, 0);
            kill(PID, SIGSTOP);
        }

    }else if(strcmp(token, "bgstart") == 0) {
        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Invalid PID given\n");
        } else {
            pid_t PID = atoi(token);
            setRunning(PID, 1);
            kill(PID, SIGCONT);
        }

    }else if(strcmp(token, "pstat") == 0) {
        printf("read pstat\n");
        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Please enter a valid PID\n");
        } else {
            pstat(atoi(token));
        }
        

    } else if(strcmp(token, "exit") == 0) {
        exit(1);
    } else {
        printf("%s: command not found\n", token);
    }

}

/**
 * Function that updates all the child process and background
 * programs based on their PID and whether or not 
 * they've been terminated, stopped, or resumed.
 * 
 * Utilizes waitpid() to give a return value, status.
 * 
 * pid_t waitpid(pid_t pid, int *status, int options);
 * pid: Process ID of our wanted process, set to -1 to wait for ANY child process.
 * status: a variable to store our resulting status call.
 * options: tracking the statuses that our process could have.
 */
void updateStatus() {
    
    for(;;) {
        int status;
        pid_t PID = waitpid(-1, &status, WCONTINUED | WNOHANG | WUNTRACED);
        if(PID > 0) {
            if(WIFCONTINUED(status)) {
                printf("Child process was resumed by delivery of SIGCONT\n");
                setRunning(PID, 1);
            } else if(WIFSTOPPED(status)) {
                printf("Child process was stopped by delivery of a signal\n");
                setRunning(PID, 0);
            } else if(WIFSIGNALED(status)) {
                printf("Child process was terminated by a signal\n");
                removeNode(PID);
            } else if(WIFEXITED(status)) {
                printf("Child terminated normally\n");
                removeNode(PID);
            }
        } else {
            return;
        }
    }
}

/**
 * Main function to initialize everything.
 * 
 * inputHandler() handles the user input, and
 * executes the corresponding commands with helper functions.
 * 
 * updateStatus() executes after every completed user input 
 * and program execution, then updates the isRunning status of 
 * processes in our linked list with their corresponding status
 * based off of the results from waitpid().
 */
int
main(int argc, char* argv[])
{
    for(;;) {
        inputHandler();
        updateStatus();
    }

	return 0;
}
