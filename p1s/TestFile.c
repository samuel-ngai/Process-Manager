#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    printf("Before first sleep\n");
    sleep(3);
    printf("Before second sleep\n");
    sleep(10);
    printf("Before third sleep\n");
    sleep(3);
    printf("Before before exit\n");
    exit(0);
}