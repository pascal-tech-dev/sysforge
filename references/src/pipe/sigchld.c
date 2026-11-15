#include<signal.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<errno.h>

int k = 0;

void child(int sno)
{
	int stat,cno;

#ifndef UNSAFE
    /* there can be multiple child terminate at the same time */
	while ((cno = waitpid(0, &stat, WNOHANG)) > 0) {
#else
	cno = wait(&stat);
#endif
	printf("Child %d died or blocked. ",cno);
	printf("Exit code is %d\n",WEXITSTATUS(stat));
    k++;
#ifndef UNSAFE
    }
#endif
    if (cno < 0) 
        perror("waitpid: ");
}

int main(int argc, char *argv[]) {
	double t;
    int i, n = 5;

    if (argc > 1) {
        n = atoi(argv[1]);
    } else {
        n = 10;
    }
	signal(SIGCHLD,child);
    /* create n children */
    for (i = 0; i < n ; i++) {
        if (fork())
            continue;
		srand(getpid()*time(NULL));
		return(rand()%127);
	}
    while (k < n) { /* not all children exitted */
        sleep(1);
    }
	return 0;
}
