#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/fcntl.h>

#define MAX(a,b)  (((a)<(b))?(b):(a))

int main() {
	int pipes[3][2];
	fd_set readset,writeset;
	int i,max,res;
	struct timeval to;
	char line[100];

	for (i=0;i<3;i++)
		pipe(pipes[i]);


	if (fork()) 
		if (fork()) 
			if (fork()) { //parent code here
				close(pipes[0][1]);
				close(pipes[1][1]);
				close(pipes[2][0]);
				/*
				fcntl(pipes[0][0],F_SETFL,O_NONBLOCK);
				fcntl(pipes[1][0],F_SETFL,O_NONBLOCK);
				fcntl(pipes[2][1],F_SETFL,O_NONBLOCK);
				*/
				/* 0 and 1 for reading, 2 for writing */
				FD_ZERO(&readset);
				FD_ZERO(&writeset);
				FD_SET(pipes[0][0],&readset);
				FD_SET(pipes[1][0],&readset);
				FD_SET(pipes[2][1],&writeset);
				to.tv_sec=5;
				to.tv_usec=0;
max=MAX(pipes[0][0],MAX(pipes[1][0],pipes[2][1]))+1;
while (1) {
	res=select(max,&readset,&writeset,NULL,&to);
	if (res<0)
		return 0;
	else if (res==0)  {
		printf("Timeout\n");
	} else {
		if (FD_ISSET(pipes[0][0],&readset)) {
			read(pipes[0][0],line,100);
			printf("Read[0]:%s\n",line);
		} 
		if (FD_ISSET(pipes[1][0],&readset)) {
			read(pipes[1][0],line,100);
			printf("Read[1]:%s\n",line);
		}
		if (FD_ISSET(pipes[2][1],&writeset)) {
			write(pipes[2][1],"parent outputs\n",15);
		}
	}
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_SET(pipes[0][0],&readset);
	FD_SET(pipes[1][0],&readset);
	FD_SET(pipes[2][1],&writeset);
	to.tv_sec=5;
	to.tv_usec=0;
}

			} else {
				srand(getpid());
				close(pipes[0][0]);
				dup2(pipes[0][1],1);
				i=0;
				while (1) {
					i++;
					sleep(rand()%20);
					printf("%d: %d\n",getpid(),i);
					fflush(stdout);
				}
			}
		else {
			srand(getpid());
			close(pipes[1][0]);
			dup2(pipes[1][1],1);
			i=0;
			while (1) {
				i++;
				sleep(rand()%20);
				printf("%d: %d\n",getpid(),i);
				fflush(stdout);
			}
		}
	else {
		srand(getpid());
		close(pipes[2][1]);
		dup2(pipes[2][0],0);
		i=0;
		while (1) {
			i++;
			sleep(rand()%20);
			fflush(stdin);
			fgets(line,100,stdin);
			printf("%d: %s\n",getpid(),line);
		}
	}

	return 0;
}

