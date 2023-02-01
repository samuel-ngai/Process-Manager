#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define TRUE 1;
#define FALSE 0;
/**
 * A linked list data structure to
 * store our processes and navigate 
 * through them
 */
typedef struct node {
	pid_t pid;
	char* programName;
    int isRunning;
	struct node *next;
} node;

node* head = NULL;
int tempPID = 123;

/**
 * Takes in PID, and program name
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
    printf("head is still %d  %s\n", head->pid, head->programName);
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

 
    // node* tempNode = head;
    // printf("tempNode is %d %n\n", tempNode->pid, tempNode->programName);
    // if(tempNode->pid == PID) {
    //     printf("head is pid\n");
    //     free(head);
    //     head = head->next;
    // } else {
    //     printf("head is not pid, iterating\n");
    //     while(tempNode->next->pid != PID) {
    //         printf("tempnode pid is %d\n", tempNode->pid);
    //         if(tempNode->next == NULL) {
    //             printf("Process with PID: %n does not exist", PID);
    //             break;
    //         }
            
    //         printf("Iterating through LL\n");
    //         tempNode = tempNode->next;

    //     }
    //     free(tempNode->next);
    //     //tempNode->next = tempNode->next->next;
    // }

    node* tempNode = head;
    printf("PID to be deleted is %d\n", PID);
    printf("tempnode specs: %d %s\n", tempNode->pid, tempNode->programName);
    //Head node is the one to be removed
    if(tempNode->pid == PID) {
        printf("head is to be removed\n");
        head = head->next;
        free(tempNode);
    } else {
        printf("need to iterate\n");
        while((tempNode != NULL) && (tempNode->pid != PID)) {
            printf("    iterating through");
            tempNode = tempNode->next;
        }
        tempNode = tempNode->next;
    }

    if(tempNode == NULL) {
        printf("PID does not exist\n");
    }
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
            addNode(tempPID, token);
            printf("added pid is %d\n", tempPID);
        }

    }else if(strcmp(token, "bglist") == 0) {
        //Print out all processes
        printf("Read bglist\n");

        printList();

    }else if(strcmp(token, "bgkill") == 0) {
        //End process with given PID, send TERM signal (Next token)
        printf("Read bgkill\n");

        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("Invalid PID given\n");
        } else {
            int removalPID = (int)token;
            printf("next token is %s\n", token);
            printf("Going to remove node  with pid %d\n", removalPID);
            removeNode(removalPID);
        }

    }else if(strcmp(token, "bgstop") == 0) {
        //Temp. stop of process with PID, send STOP signal (Next token)
        printf("Read bgstop\n");

    }else if(strcmp(token, "bgstart") == 0) {
        //Restart previously stopped process with PID, send CONT signal  
        printf("Read bgstart\n");

    }else if(strcmp(token, "pstat") == 0) {
        printf("Read pstat\n");

    } else if(strcmp(token, "exit") == 0) {
        exit(1);
    } else {
        printf("Invalid commands\n");
    }

    
    
}

int
main(int argc, char* argv[])
{
    for(;;) {
        char* inputCommand[200];
        inputHandler();
        tempPID++;
    }

	

	return 0;
}
