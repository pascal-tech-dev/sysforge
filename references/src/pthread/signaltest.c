#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<signal.h>
#include<execinfo.h>

__thread int globaldata[1024000];

void handler(int signo, siginfo_t *info, void *p)
{
           void *buffer[10];
           char **strings;
	   int nptrs, j;

	if (signo == SIGFPE || signo == SIGSEGV || signo == SIGBUS || signo == SIGILL) 
		printf("thread: %ld, si_addr %p\n", pthread_self(), info->si_addr);

           nptrs = backtrace(buffer, 10);
           printf("backtrace() returned %d addresses\n", nptrs);

           strings = backtrace_symbols(buffer, nptrs);
           if (strings == NULL) {
               perror("backtrace_symbols");
               exit(EXIT_FAILURE);
           }

           for (j = 0; j < nptrs; j++)
               printf("%s\n", strings[j]);


	
	exit(1);
}


void *f(void *arg) {
	int id = *(int *)arg, i;
	sigset_t set;

	sigemptyset(&set);
        sigaddset(&set, SIGSEGV);
	//pthread_sigmask(SIG_BLOCK, &set, NULL);

	free(arg);
	printf("id: %d, tid:%ld\n", id, pthread_self());
	for (i = 0; i < 100; i++) {
		if (i == 10) {
			arg = (void *) 0xdead;
			printf("%d \n", *(int *) arg);
		}
		sleep(1);
	}
}

void *g(void *arg) {
	int id = *(int *)arg, i;
	free(arg);
	printf("id: %d, tid:%ld\n", id, pthread_self());
	for (i = 0; i < 100; i++) {
		sleep(1);
	}
}


int main(int argc,char *argv[]) 
{
	pthread_t tr[2];
	int *p;
	struct sigaction act;
	sigset_t set;

	memset(&act, 0, sizeof(struct sigaction));

	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART | SA_SIGINFO;
	act.sa_sigaction = handler;
	sigaction(SIGSEGV, &act, NULL);

	p = malloc(sizeof(int));
	*p = 0;
	pthread_create(tr,NULL,f,(void *) p);
	p = malloc(sizeof(int));
	*p = 1;
	pthread_create(tr,NULL,g,(void *) p);

	sigemptyset(&set);
        sigaddset(&set, SIGSEGV);
	pthread_sigmask(SIG_BLOCK, &set, NULL);

	pthread_join(tr[0],NULL);
	pthread_join(tr[1],NULL);

	return 0;
}
