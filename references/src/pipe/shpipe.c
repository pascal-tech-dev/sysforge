#include<unistd.h>
#include<wait.h>
#include<stdio.h>

int main() {

	int p,q,r;
	int fd[2];

	pipe(fd);
	if (p=fork()) {
	       if (q = fork()) {
				close(fd[0]);
				close(fd[1]);
				wait(&r);
				wait(&r);
		   } else {
				close(fd[1]);
				dup2(fd[0],0);
				close(fd[0]);
				execl("/usr/bin/tr", "tr", "/a-z/", "/A-Z/", NULL);
			}
	} else {
		close(fd[0]);
		dup2(fd[1],1);
		close(fd[1]);
		execl("/bin/ls", "ls", "-l", NULL);
	}

	return 0;
}


