#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

extern char *const *environ;

int execvpeMy(const char *path, char *const argv[], char *const envp[]) {
	char *const *copyEnv = environ;
	environ = envp;
	execvp(path, argv);
	environ = copyEnv;

	return 0;
}

int main() {

	char *const envp[] = {"LOGNAME=eugene", "LANG=C", "PATH=/bin",
						  "HOME=/home/eugene", NULL};
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("error fork");
		exit(1);

	} else if (pid == 0) {
		char *const argv[] = {"env", NULL};
		execvpeMy("env", argv, envp);
		perror("execvpe return control");
		exit(1);
	}
	wait(NULL);
	printf("\nEverything is fine!\n");
	return 0;
}