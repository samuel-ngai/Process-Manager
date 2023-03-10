#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#define TRUE 1;
#define FALSE 0;

/**
 * A linked list data structure to store our processes and navigate through them.
 * PID: Process ID 
 * command: Name of program 
 * isRunning: True or false if its currently running or not
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
    char actualPath[128];
    while(iteratorNode != NULL) {
        realpath(iteratorNode->command, actualPath);
        printf("%d: %s\n", iteratorNode->pid, actualPath);
        iteratorNode = iteratorNode->next;
    }
}

/**
 * Given an input PID, we will remove the corresponding node in our linked list,
 * and return an error if the node does not exist.
 */

void removeNode(pid_t PID) {

    node* iteratorNode = head;

    if(iteratorNode == NULL) {
        return;
    }

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

/**
 * Function that reads /proc/[PID]/stat file and prints out:
 * 
 *  1. comm: The filename of the executable, in parentheses. 
 *  2. state: One of the following characters, indicating process state.
 *      R - Running
 *      S - Sleeping in an interruptible wait
 *      D - Waiting in uninterruptible disk sleep
 *      Z - Zombie
 *      T - Stopped
 *      t - Tracing stop
 *      W - Paging
 *      X - Dead
 *      x - Dead
 *      K - Wakekill
 *      W - Waking
 *      P - Parked
 *  3. utime: Amount of time that this process has been scheduled in user mode.
 *  4. stime: Amount of time that this process has been scheduled in kernel mode.
 *  5. rss: Resident set size: Number of pages the process has in real memory.
 *
 * Returns an error message if the FILE object is NULL.
 */
void readStatFile(char* procPath) {

    FILE* statFile = fopen(procPath, "r");   
    ssize_t read;
    size_t len = 0;
    char* line = NULL;
    char* data = NULL;

    if(statFile == NULL) {
        printf("Failed to access file\n");
        return;
    } 
    int i = 0;
    while((read = getline(&line, &len, statFile)) != NULL) {

        if(i == 1) {
            data = strtok(line, " ");
            break;
        }
        i++;
    }
    for(int i = 0; i<25; i++) {
        if(i == 1) {
            printf("comm: %s\n", data);
        }
        if(i == 2) {
            printf("state: %s\n", data);
        }
        if(i == 13) {
            printf("utime: %s\n", data);
        }
        if(i == 14) {
            printf("stime: %s\n", data);
        }
        if(i == 23) {
            printf("rss: %s\n", data);
        }
        data = strtok(NULL, " ");
    }
    fclose(statFile);
}

/**
 * Function that reads the /proc/[PID]/status file and prints out:
 * 
 *  1. voluntary ctxt switches: Number of voluntary context switches
 *  2. nonvoluntary ctxt switches: Number of involuntary context switches.
 *  
 * Returns an error message if the FILE object is NULL.
 */
void readStatusFile(char* procPath) {

    FILE* statusFile = fopen(procPath, "r");
    ssize_t read;
    size_t len = 0;
    char* line = NULL;
    if(statusFile == NULL) {
        printf("Failed to access file\n");
    } 
    int i = 0;
    while((read = getline(&line, &len, statusFile)) != NULL) {
        if(i == 53) {
            printf("%s", line);
        }
        if(i == 54) {
            printf("%s", line);
            break;
        }
        i++;
    }
    fclose(statusFile);
}

/**
 * Function that checks if given a valid PID, sends the corresponding 
 * /proc/[PID]/stat & /proc/[PID]/status paths to readStatFile() & 
 * readStatusFile() for parsing and printing.
 */
void pstat(pid_t PID) {
    if(nodeExists(PID)) {
        
        char status[200];
        char stat[200];
        sprintf(stat, "/proc/%d/stat", PID);
        sprintf(status, "/proc/%d/status", PID);

        readStatFile(stat);
        readStatusFile(status);

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
void inputHandler(char* input) {

    char* token = strtok(input, " ");
    char* commands[10];

    if(strcmp(token, "bg") == 0) {
        token = strtok(NULL, " ");
        char* arguments = token;

        if(token == NULL) {
            printf("No program given\n");
        } else {
            pid_t PID = fork();
            if(PID > 0) { 
                addNode(PID, token);
                printf("Started new process with PID: %d\n", PID);
                sleep(1);
            } else if(PID == 0) {   
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
            pid_t PID = atoi(token);
            removeNode(PID);
            kill(PID, SIGTERM);
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
        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Please enter a valid PID\n");
        } else {
            pstat(atoi(token));
        }
        
    } else if(strcmp(token, "exit") == 0) {
        exit(1);
    } else {
        printf("PMan:> %s: command not found\n", token);
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

/**
 * Main function to initialize everything.
 * 
 * If the input given by the user is empty, then the program
 * will prompt the user again for a non-empty input, and pass it
 * to the inputHandler.
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
        char* myCommand = readline("PMan: >");
        if(strcmp(myCommand, "") == 0) {
            printf("No input given\n");
        } else {
            inputHandler(myCommand);
        }
        usleep(10000);
        updateStatus();
    }

	return 0;
}
