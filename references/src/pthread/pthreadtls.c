#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

int x = 5;

__thread int y = 4;

pthread_key_t mkey[3];

void create_keys()
{
	int i;
	for (i=0; i<3; i++) 
		pthread_key_create(mkey+i, free);
}

void *f(void *param) 
{
	int i;
	double *p, *q, *r;

	printf("(%x) x:%x  y:%x\n", pthread_self(),&x, &y);
	for (i=0; i<3; i++)  {
		if (pthread_getspecific(mkey[i]) == NULL) {
			q = malloc(sizeof(double));
			pthread_setspecific(mkey[i], q);
		}
	}

	p = pthread_getspecific(mkey[0]);
	q = pthread_getspecific(mkey[1]);
	r = pthread_getspecific(mkey[2]);

	/* computation here */
	*r = *p+*q;
	
	return NULL;
}


int main() {
	pthread_t t1, t2;
	
	create_keys();
	pthread_create(&t1, NULL, f, NULL);
	pthread_create(&t2, NULL, f, NULL);
	printf("(%x) x:%x  y:%x\n", pthread_self(),&x, &y);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}


