#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

static void handler(int sig, siginfo_t *si, void *uc) {
	printf("Caught signal %d, sending PID: %ld\n", sig, (long)si->si_pid);
}

void *foo(void * num)
{
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	printf("foo\n");
	if(sigaction(SIGUSR1, &sa, 0) == -1 || sigaction(SIGQUIT, &sa, 0) ==  -1)
	{
		perror("sigaction");
		exit(1);
	}
	while(1)
		pause();
}

int main()
{
	pthread_t thread;
	pthread_create(&thread, NULL, foo, NULL);
	pthread_join(thread, NULL);
	return 0;
}
