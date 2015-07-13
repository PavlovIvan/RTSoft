#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int flag=0;
char pass[]="123";
char str[20];

void *foo1(void * num)
{
	printf("Enter pass\n");
	scanf("%s", str);
	flag=1;
}

void *foo2(void * num)
{
	while(!flag);
	if(!strcmp(str , pass))
		printf("true\n");
	else
		printf("false\n");
}

int main()
{
	pthread_t thread1, thread2;
	int rc1, rc2;
	rc1 = pthread_create(&thread1, NULL, foo1, NULL);
	rc2 = pthread_create(&thread2, NULL, foo2, NULL);
	if(rc1 || rc2)
	{
		printf("error!\n");
		exit(-1);
	}	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	return 0;
}
