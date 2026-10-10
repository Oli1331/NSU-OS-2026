#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		perror("error fork");
		exit(1);
	}
	else if (pid == 0)
	{
		execlp("cat", "cat", "./text.txt", NULL);
		perror("error exec");
		exit(1);
	}
	wait(NULL);
	printf("\n\n'какой-либо текст.'\n");

	return 0;
}