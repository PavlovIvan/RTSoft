#include "bin_sem.h"
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>

#define KEY ((key_t)(1234))
#define SIZE 1024*1024

char buff[SIZE];

int main()
{
	key_t key = ftok("file1.c", 1);
	int semid = binary_semaphore_allocation(key, 0666 | IPC_CREAT);
	int i=0;
	FILE * fd;
	char *shared_memory;
	int id = shmget(KEY, SIZE, 0);
	if (id < 0 || semid <= 0) 
		exit(1);
	shared_memory = (char *)shmat(id, 0, 0);
	fd = fopen ( "10mb_file_copy" , "wb" );
	for(i=0;i<10;++i)
	{
		binary_semaphore_take(semid);
		printf("2: take semaphore; i=%d\n", i);
		memcpy(buff, shared_memory, sizeof(buff));
		fwrite(buff, sizeof(buff), 1, fd);
		sleep(10);
		binary_semaphore_free(semid);
		printf("2: free semaphore; i=%d\n", i);
	}
	shmdt(shared_memory);
	close(fd);
	exit(0);
}
