#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Process Lifecycle\n");
    printf("-------------------\n");

    printf("Process ID: %ld\n", (long)getpid());
    printf("Parent Process ID: %ld\n", (long)getppid());

    fflush(stdout);

    printf("Sleeping for 30 seconds...\n");
    fflush(stdout);

    unsigned int remaining = sleep(30);

    if (remaining > 0) {
        printf("Sleep interrupted after %u seconds\n", remaining);
    } else {
        printf("Sleep completed successfully\n");
    }

    printf("Process terminating normally\n");
    return 0;
}