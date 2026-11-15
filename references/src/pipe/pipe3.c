#include<unistd.h>
#include<stdio.h>

int main() {

	int p,i;
	int fd1[2],fd2[2];
	char *messages[]={"3+4\n","5*6\n","10+100-(4*4)\n"};
	char result[100];

	pipe(fd1);
	pipe(fd2);
	if (p=fork()) {
		close(fd1[1]);
		close(fd2[0]);
		dup2(fd2[1],2);
		dup2(fd1[0],0);
		close(fd2[1]);
		close(fd1[0]);
		for (i=0;i<3;i++) {
			fprintf(stderr,messages[i]);
			fflush(stderr);
			if (gets(result)) {
				printf("%s\n",result);
			}
		}
		close(2); /* so that child gets an EOF */

	} else {
		close(fd1[0]);
		close(fd2[1]);
		dup2(fd1[1],1);
		dup2(fd2[0],0);
		close(fd1[1]);
		close(fd2[0]);
		execl("/usr/bin/bc","bc","-q",(char *)0);
	}

	return 0;
}


