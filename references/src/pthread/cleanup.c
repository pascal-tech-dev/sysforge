
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void clean2(void *arg) {
	fprintf(stderr,"(2) cleaning %x\n",arg);
	free(arg);
}

void clean1(void *arg) {
	fprintf(stderr,"(1) cleaning %x\n",arg);
	free(arg);
}

void dosomethingelse() {
	char *heap2;
	int i;
	heap2=malloc(sizeof(int)*100);
	pthread_cleanup_push(clean2,heap2);

	for (i=0;i<100;i++) {
		heap2[i]++;
		sleep(1);
		pthread_testcancel();
	}
	pthread_cleanup_pop(1);
}

void *dosomething(void *arg) {
	char *heapvar;
	int i;
	heapvar=malloc(sizeof(int)*100);
	pthread_cleanup_push(clean1,heapvar);
	while(1) 
		for (i=0;i<100;i++) {
			heapvar[i]++;
			dosomethingelse();
			pthread_testcancel();
		}
	pthread_cleanup_pop(1);
}


int main(int argc,char *argv[]) 
{
	pthread_t tr[5];
	int i,r;

	for (i=0;i<5;i++)
		pthread_create(&tr[i],NULL,dosomething,NULL);
	sleep(10);
	for (i=0;i<5;i++)
		pthread_cancel(tr[i]);
	for (i=0;i<5;i++)
		pthread_join(tr[i],NULL);

	return 0;
}
