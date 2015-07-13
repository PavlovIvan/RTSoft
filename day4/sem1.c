#include "bin_sem.h"
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>

#define KEY ((key_t)(1234))
#define SIZE 1024*1024

char buff[SIZE];

int main()
{
	key_t key = ftok("sem1.c", 1);
	int semid = binary_semaphore_allocation(key, 0666 | IPC_CREAT);
	int i=0;
	FILE * fd;
	char *shared_memory;
	int id = shmget(KEY, SIZE, IPC_CREAT | 0666);
	if (id < 0) 
		exit(1);
	shared_memory = (char *)shmat(id, 0, 0);
	fd = fopen ( "10mb_file" , "rb" );
	binary_semaphore_initialize(semid);
	for(i=0;i<10;++i)
	{
		fread(buff, 1, sizeof(buff), fd);
		binary_semaphore_take(semid);
		printf("1: take semaphore; i=%d\n", i);
		memcpy(shared_memory, buff, sizeof(buff));
		binary_semaphore_free(semid);
		printf("1: free semaphore; i=%d\n", i);
		if(i==0)
		{
			printf("Press enter to continue"); getchar();
		}
	}
	shmdt(shared_memory);
	close(fd);
	exit(0);
}
