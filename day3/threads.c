#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 10

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

int global_i = 1;
long num_last = -1;

void *print_hello(void * num)
{
	long t_num;
	int i;
	pthread_t id;
	t_num = (long) num;
	pthread_mutex_lock(&count_mutex);
	i = global_i;
	++global_i;
	pthread_mutex_unlock(&count_mutex);
	sleep(i);
	printf("thread #%ld was sleeping %d sec\n", t_num, i);
	pthread_mutex_lock(&count_mutex);
	if(i != NUM_THREADS)
	{
		pthread_cond_wait(&count_threshold_cv, &count_mutex);
		printf("thread #%ld waited on %ld thread. Done.\n", t_num, num_last);
	}
	else
	{
		num_last = t_num;
		pthread_cond_broadcast(&count_threshold_cv);
		printf("thread #%ld done\n", t_num);
	}
	pthread_mutex_unlock(&count_mutex);	
}

int main()
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init (&count_threshold_cv, NULL);
	for(t=0;t<NUM_THREADS;++t)
	{
		printf("create %ld\n", t);
		rc = pthread_create(&threads[t], NULL, print_hello, (void *)t);
		if(rc)
		{
			printf("error!\n");
			exit(-1);
		}
	}
	for(t=0;t<NUM_THREADS;++t)
	{
		pthread_join(threads[t], NULL);	
	}
	printf ("Main thread waited on all threads. Done.\n", NUM_THREADS);
	pthread_mutex_destroy(&count_mutex);
	pthread_cond_destroy(&count_threshold_cv);
	return 0;
}
