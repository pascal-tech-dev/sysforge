#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdio.h>
typedef struct uinfo {
	char name[20];
	int accesscount;
} userinfo;

userinfo rec;


int main() {
	struct flock fl;
	int fd,n,r;

	//fd=open("/dev/zero",O_RDWR);
	fd=open("userdb.txt",O_RDWR);

	scanf("%d",&n);
	fl.l_type=F_RDLCK; fl.l_whence=SEEK_SET; 
	fl.l_start=(n-1)*sizeof(userinfo); fl.l_len=sizeof(userinfo);

	r=fcntl(fd,F_SETLK,&fl);
	if (r<0) {
		perror("read lock");
		return (-1);
	} 
	lseek(fd,(n-1)*sizeof(userinfo),SEEK_SET);
	read(fd,&rec,sizeof(rec));
	rec.accesscount++;
	fl.l_type=F_WRLCK;
	r=fcntl(fd,F_SETLK,&fl);
	if (r<0) {
		perror("write lock");
		return (-1);
	} 
	sleep(20);
	lseek(fd,-1*sizeof(userinfo),SEEK_CUR);
	write(fd,&rec,sizeof(rec));
	fl.l_type=F_UNLCK;
	r=fcntl(fd,F_SETLK,&fl);

	return 0;
}

