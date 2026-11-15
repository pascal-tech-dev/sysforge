#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

__thread int globaldata[10024000];
//int globaldata[10024];

void *helloworld(void *arg) {
	printf("Hello, I am: pid %u tid %u \n", getpid(),(unsigned)pthread_self());
	printf("address of globaldata is: %p\n", globaldata);
	sleep(60);
}


int main(int argc,char *argv[]) 
{
	pthread_t tr[10];
	int i,r;

	printf("main global %p\n", globaldata);
	sleep(20);
	for (i=0;i<10;i++)
		pthread_create(&tr[i],NULL,helloworld,NULL);
	for (i=0;i<10;i++)
		pthread_join(tr[i],NULL);

	return 0;
}
