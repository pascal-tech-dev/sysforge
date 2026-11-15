#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
typedef struct uinfo {
	char name[20];
	int accesscount;
} userinfo;

userinfo rec;


int main() {
	struct flock fl;
	int fd,r;

	fd=open("userdb.txt",O_RDWR);

	fl.l_type=F_WRLCK; 
	fl.l_whence=SEEK_SET; 
	fl.l_len=sizeof(userinfo);
	if (fork()) {
		fl.l_start=0;
		r=fcntl(fd,F_SETLK,&fl);
		if (r<0) {
			perror("parent: lock 0");
			return (-1);
		} 
		sleep(1);
		fl.l_start=sizeof(userinfo);
		r=fcntl(fd,F_SETLKW,&fl);
		if (r<0) {
			perror("parent: lock 1");
			return (-1);
		} 
	} else {
		fl.l_start=sizeof(userinfo);
		r=fcntl(fd,F_SETLK,&fl);
		if (r<0) {
			perror("child: lock 1");
			return (-1);
		} 
		sleep(1);
		fl.l_start=0;
		r=fcntl(fd,F_SETLKW,&fl);
		if (r<0) {
			perror("child: lock 0");
			return (-1);
		} 
	}

	return 0;
}

