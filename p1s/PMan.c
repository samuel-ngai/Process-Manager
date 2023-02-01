#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


/**
 * A linked list data structure to
 * store our processes and navigate 
 * through them
 */
typedef struct node {
	pid_t pid;
	char* programName;
	struct node *next;
} node;

node* head = NULL;


/**
 * Takes in PID, and program name
 * adds to end of process linked list
 */ 
void addToList(pid_t PID, char* program) {
    node* process = (struct node*)malloc(sizeof(struct node));

    process->pid = PID;
    process->programName = program;
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

void printList() {
    node* tempNode = head;
    while(tempNode != NULL) {
        printf("%d: %s\n", tempNode->pid, tempNode->programName);
        tempNode = tempNode->next;
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
    char *prompt  = "PMan: >";
    char *input;
    int AssigneePID = 1;
    //input = readline(prompt);

    char* myCommand = readline("PMan: >");



    char* token = strtok(myCommand, " ");

    if(strcmp(token, "bg") == 0) {
        //Add second token (program) to linked list (processes)
        printf("Read bg\n");
        token = strtok(NULL, " ");
        if(token == NULL) {
            printf("No program given\n");
        } else {
            int tempPID = 123;
            addToList(tempPID, token);
        }

    }else if(strcmp(token, "bglist") == 0) {
        //Print out all processes
        printf("Read bglist\n");

        printList();

    }else if(strcmp(token, "bgkill") == 0) {
        //End process with given PID, send TERM signal (Next token)
        printf("Read bgkill\n");

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
    }

	

	return 0;
}
