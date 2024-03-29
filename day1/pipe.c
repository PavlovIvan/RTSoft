#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *str_arr[1024];
int n=0;

char *get_pname(char *str)
{
	int i;
	int len = strlen(str);
	for(i=len;i>=0 && str[i]!=' ';--i);
	return &str[i+1];
}

int cmp(const void *a, const void *b)
{
	char *pa;
	char *pb;
	pa=get_pname(*(char **)a);
	pb=get_pname(*(char **)b);
	return strcmp(pa, pb);
}

void rd_pipe(int pfd)
{
	char ch;
	int i=0;
	int j=0;
	int len;
	char buff[1024];
	while(read(pfd, &ch, 1) == 1)
	{
		if(ch=='\n')
		{
			buff[i]=0;
			len = strlen(buff);
			str_arr[j]=(char *)malloc(len+1);
			strcpy(str_arr[j], buff);
			++j;
			i=0;
		}
		else
		{
			buff[i]=ch;
			++i;		
		}
		//putchar(ch);
	}
	n=j-2;
}

int main()
{
	pid_t pid;
	int pfd[2];
	int i;
	if (pipe (pfd))
	{
		perror("pipe()");
		return ;
	}
	pid = fork ();
	if (pid == 0)//ребенок
	{
		close(pfd[0]);
		dup2(pfd[1], 1);
		dup2(pfd[1], 2);
		execl("/bin/ls", "ls", "-l", NULL);
		close(pfd[1]);
		return 0;
	}
	else if (pid < 0)
	{
		perror("fork()");
		return 1;
	}
	else//родитель
	{
		close (pfd[1]);
		rd_pipe(pfd[0]);
		qsort(&str_arr[1], n, sizeof(char *), cmp);
		for(i=1;str_arr[i];++i)
		{
			printf("%d) %s\n", i, str_arr[i]);
			free(str_arr[i]);
		}
		free(str_arr[0]);
		return 0;
	}
}
