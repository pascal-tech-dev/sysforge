#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t forks[5];

void philosopher(void *ip)
{
	pthread_mutex_t *sol,*sag;
	int i=*(int *) ip;
	int z;

	printf("philosopher %d thread id %u started\n", i,
  			pthread_self()); 
	sol=i==0?forks+4:forks+(i-1);
	sag=forks+i;
	srand(i+time(NULL));
	for (z=0;z<100;z++) {
//		usleep(rand()%10);
		if (i % 2 == 0){
			pthread_mutex_lock(sol);
			pthread_mutex_lock(sag);
		} else {
			pthread_mutex_lock(sag);
			pthread_mutex_lock(sol);
		}
		printf("Philosopher %d is eating %d\n",i,z);
		//usleep(10000); //rand()%30000+10000);
		printf("Philosopher %d finished eating %d\n",i,z);
		pthread_mutex_unlock(sol);
		pthread_mutex_unlock(sag);
	}
}

int main() 
{
	pthread_t phils[5];
	int phids[5]={0,1,2,3,4};
	int i,n;
	
	for (i=0;i<5;i++) {
		if (pthread_mutex_init(&(forks[i]),NULL)) {
			perror("Creating mutex");
			return(-1);
		}
	}
	for (i=0;i<5;i++) {
		if (pthread_create(&(phils[i]), NULL, philosopher,  (void *) &(phids[i])) )
			perror("Create failed");
	}
	for (i=0;i<5;i++) {
		pthread_join(phils[i],NULL);
	}
	sleep(1);
}
