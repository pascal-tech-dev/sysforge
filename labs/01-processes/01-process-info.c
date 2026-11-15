
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    char cwd[4096];

    printf("Process Information\n");
    printf("-------------------\n");

    // Process ID and Parent Process ID
    printf("PID:  %ld\n", (long)getpid());
    printf("PPID: %ld\n", (long)getppid());

    // User ID and Effective User ID
    printf("UID:  %ld\n", (long)getuid());
    printf("EUID: %ld\n", (long)geteuid());

    // Group ID and Effective Group ID
    printf("GID:  %ld\n", (long)getgid());
    printf("EGID: %ld\n", (long)getegid());

    // Process Group ID and Session ID
    printf("PGID: %ld\n", (long)getpgrp());
    printf("SID:  %ld\n", (long)getsid(0));

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    printf("CWD:  %s\n", cwd);

    return 0;
}
