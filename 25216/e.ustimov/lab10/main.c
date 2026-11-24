#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	pid_t pid;
	if (argc < 2)
		return 0;

	if ((pid = fork()) < 0)
	{
		perror("fault fork");
		exit(1);
	}
	else if (pid == 0)
	{

		execvp(argv[1], (argv + 1));
		perror("fault exec");
		exit(1);
	}
	int res_child;
	wait(&res_child);
	printf("\nchild's exit code: %d\n", res_child);

	return 0;
}