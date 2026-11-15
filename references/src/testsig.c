#include<signal.h>
#include<stdio.h>
int increment=1;

static void usrhandler(int sno, siginfo_t *inf, void *context)
/*void usrhandler(int sno)*/
{
	if (inf != NULL ) 
	printf("signal %02d taken from pid %d, user %d\n",
			inf->si_signo, inf->si_pid, inf->si_uid);
	if (sno==SIGUSR1) {
		printf("USR1 signal taken, counting backward\n");
		increment=-1;
	} else  if (sno==SIGUSR2) {
		printf("USR2 signal taken, counting forward\n");
		increment=1;
	} else if (sno==SIGINT) {
		printf("Ctrl-C pressed, ignoring\n");
	}
	sleep(3);
}

int main() {
	struct sigaction act, oact;
	int i=0;
	act.sa_sigaction=usrhandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags=SA_SIGINFO;
	sigaction(SIGUSR1,&act,NULL);
	sigaction(SIGUSR2,&act,NULL);
	sigaction(SIGINT,&act,NULL);
	for (;;i+=increment) {
		printf("%10d\n",i);	
		sleep(5);
	}
	return 0;
}
