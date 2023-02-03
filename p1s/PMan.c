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
 * A linked list data structure to
 * store our processes and navigate 
 * through them
 * PID: Process ID 
 * programName: Name of program 
 * isRunning:  True or false if its currently running or not
 * next: reference to next node;
 */
typedef struct node {
	pid_t pid;
	char* programName;
    int isRunning;
	struct node *next;
} node;

node* head = NULL;

/**
 * Takes in PID and program name,
 * adds to end of process linked list
 */ 
void addNode(pid_t PID, char* program) {
    node* process = (struct node*)malloc(sizeof(struct node));
    
    process->pid = PID;
    process->programName = program;
    process->isRunning = TRUE;
    process->next = NULL;

    if(head == NULL) {
        head = process;
    } else {
        node* tempNode = head;
        while(tempNode->next != NULL) {
            tempNode = tempNode->next;
        }
        tempNode->next = process;
    }

    printf("process: %s, PID %d\n", process->programName, process->pid);

}

/**
 * Function to loop through our linked
 * list and print all PID and program names
 */ 
void printList() {
    node* tempNode = head;
    while(tempNode != NULL) {
        printf("%d: %s\n", tempNode->pid, tempNode->programName);
        tempNode = tempNode->next;
    }
    printf("-------------\n");
    if(head == NULL) {
        printf("head is null\n");
    } else {
        printf("head is still %d  %s\n", head->pid, head->programName);
    }
    printf("-------------\n");
}

/**
 * Given an input PID, we will
 * remove the corresponding node 
 * in our linked list,
 * and return an error if the node
 * does not exist
 */

void removeNode(pid_t PID) {

    node* tempNode = head;
    printf("PID to be deleted is %d\n", PID);
    printf("tempnode specs: %d %s\n", tempNode->pid, tempNode->programName);
    if(tempNode->pid == PID) {
        printf("head is to be removed\n");
        head = head->next;
        free(tempNode);
    } else {
        tempNode = tempNode->next;
        node* prevTemp = head;
        printf("need to iterate\n");
        while((tempNode != NULL) && (tempNode->pid != PID)) {
            printf("at node with %d %s\n", tempNode->pid, tempNode->programName);
            printf("    iterating through\n");
            tempNode = tempNode->next;
            prevTemp = prevTemp->next;
        }
        if(tempNode == NULL) {
            printf("Invalid PID\n");
            return;
        }

        prevTemp->next = tempNode->next;
        free(tempNode);

    }

    if(tempNode == NULL) {
        printf("PID does not exist\n");
    }

}

/**
 * Returns node object with a specific PID
 */
node* getNode(pid_t PID) {
    node* tempNode = head;

    if(tempNode->pid == PID) {
        return tempNode;
    }  else {
        while(tempNode != NULL && tempNode->pid != PID) {
            tempNode = tempNode->next;
        }
        return tempNode;
    }
}

/**
 * Given a PID, set the isRunning LL node 
 * to FALSE after sending SIGSTOP signal
 */
void stopNode(pid_t PID) {
    node* tempNode = head;
    while(tempNode->pid != PID) {
        tempNode = tempNode->next;
    }
    tempNode->isRunning = FALSE;
}

/**
 * Given a  PID, set the isRunning LL node
 * to TRUE after sending SIGCONT signal
 */
void startNode(pid_t PID) {
    node* tempNode = head;
    while(tempNode->pid != PID) {
        tempNode = tempNode->next;
    }
    tempNode->isRunning = TRUE;
}

/**
 * Function that counts total number
 * of nodes present in LL
 * Returns int
 */ 
int* bgcount() {
    node* tempNode = head;
    int counter = 0;
    while(tempNode != NULL) {
        counter++;
        tempNode = tempNode->next;
    }
    printf("Total background jobs: %d\n", counter);
    return counter;
}

    /**
	 * Get user input
	 * 
	 * How it works:
	 * Given an input, program will tokenize string
	 * and depending on input (bg, bglist, ...etc.),
	 * it will work with the linked list to perform
	 * the given actions.
	 */
void inputHandler() {

    char* myCommand = readline("PMan: >");
    char* token = strtok(myCommand, " ");

    if(strcmp(token, "bg") == 0) {
        //Add second token (program) to linked list (processes)
        printf("Read bg\n");
        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("No program given\n");
        } else {
            
            pid_t PID = fork();

            if(PID > 0) {
                addNode(PID, token);
                printf("added pid is %d\n", PID);
                
                char target[200];
                char* executableTag = "./";
                char* executableName = malloc(strlen(token)+2);
                strcpy(executableName, token);
                printf("copied token to char\n");
                printf("%s\n", executableName);
                printf("%s\n", executableTag);

                strcpy(target, executableTag);
                strcat(target, executableName);

                int size = strlen(target);
                target[size-1] = '\0';
                target[size-2] = '\0';
                
                //strcat(executableTag,executableName);
                printf("About to execute:  %s\n", target);
                system(target);
                sleep(1);
                free(executableName);
            } else if(PID == 0) {
                printf("failed\n");
                exit(1);
            } else {
                printf("Fork error\n");
            }
            printf("Generated PID is %d\n", PID);
        }

    }else if(strcmp(token, "bglist") == 0) {
        //Print out all processes
        printf("Read bglist\n");

        printList();
        bgcount();

    }else if(strcmp(token, "bgkill") == 0) {
        //End process with given PID, send TERM signal (Next token)
        printf("Read bgkill\n");

        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Invalid PID given\n");
        } else {
            pid_t removalPID = atoi(token);
            printf("next token is %s\n", token);
            printf("Going to remove node  with pid %d\n", removalPID);
            removeNode(removalPID);
        }
    

    }else if(strcmp(token, "bgstop") == 0) {
        //Temp. stop of process with PID, send STOP signal (Next token)
        printf("Read bgstop\n");

        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Invalid PID given\n");
        } else {
            pid_t PID = atoi(token);
            stopNode(PID);
            kill(PID, SIGSTOP);
        }

    }else if(strcmp(token, "bgstart") == 0) {
        //Restart previously stopped process with PID, send CONT signal  
        printf("Read bgstart\n");

        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Invalid PID given\n");
        } else {
            pid_t PID = atoi(token);
            kill(PID, SIGCONT);
        }

    }else if(strcmp(token, "pstat") == 0) {
        printf("Read pstat\n");



    } else if(strcmp(token, "exit") == 0) {
        exit(1);
    } else {
        printf("%s: command not found\n", token);
    }

}

int
main(int argc, char* argv[])
{
    for(;;) {
        char* inputCommand[200];
        inputHandler();
    }

	return 0;
}
