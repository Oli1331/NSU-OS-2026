#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_term;

void return_settings() {
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
	printf("\nSettings returned\n");
}

void sig_handler(int signo) { exit(0); }

int main() {

	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) != 0) {
		perror("can't get settings terminal\n");
		return 1;
	}
	orig_term = term;
	if (atexit(return_settings) != 0) {
		perror("can't set exit func\n");
		return 1;
	}
	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		perror("can't set signal handler\n");
		return 1;
	}

	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	term.c_lflag &= ~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	char c;
	while (1) {
		printf("You agree? ");
		fflush(stdout);
		read(STDIN_FILENO, &c, 1);
		if (c == 'y') {
			printf("\nGreat!\n");
			break;
		} else if (c == 'n') {
			printf("\nOkay..\n");
			break;
		} else {
			printf(" - What? \n");
		}
	}

	return 0;
}