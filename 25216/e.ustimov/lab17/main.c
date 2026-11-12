#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#define MAX_LEN_LINE 5

struct termios orig;

void return_settings_terminal() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig); }

int main() {

	if (tcgetattr(STDIN_FILENO, &orig) != 0) {
		perror("can't get settings terminal");
		return 1;
	}
	struct termios term = orig;
	if (atexit(return_settings_terminal) != 0) {
		perror("can't set func in atexit");
		return 1;
	}
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
	int cnt_sym_line = 0;
	char line[MAX_LEN_LINE + 1];
	char c;
	while (read(STDIN_FILENO, &c, 1) != 0) {
		switch (c) {
		case 127:
		case ('H' & 0x1F):
			if (cnt_sym_line > 0) {
				printf("\b \b");
				cnt_sym_line--;
			}
			break;
		case ('U' & 0x1F):
			putchar('\r');
			while (cnt_sym_line > 0) {
				putchar(' ');
				cnt_sym_line--;
			}
			putchar('\r');
			break;
		case ('W' & 0x1F):
			cnt_sym_line--;
			while (cnt_sym_line >= 0 && isspace(line[cnt_sym_line])) {
				printf("\b \b");
				cnt_sym_line--;
			}
			while (cnt_sym_line >= 0 && !isspace(line[cnt_sym_line])) {
				printf("\b \b");
				cnt_sym_line--;
			}
			cnt_sym_line++;
			break;
		case ('D' & 0x1F):
			if (cnt_sym_line == 0)
				return 0;
			break;

		case ('\n' & 0x1F):
			putchar('\n');
			cnt_sym_line = 0;
			break;

		default:
			if (cnt_sym_line >= MAX_LEN_LINE) {
				if (!isspace(c)) {
					int ptr = cnt_sym_line;
					while (ptr > 0 && !isspace(line[ptr - 1])) {
						ptr--;
					}

					if (ptr == 0) {
						putchar('\n');
						cnt_sym_line = 0;
					} else {
						// ptr += 1;
						cnt_sym_line--;
						while (cnt_sym_line >= 0 &&
							   !isspace(line[cnt_sym_line])) {
							printf("\b \b");
							cnt_sym_line--;
						}
						cnt_sym_line = 0;
						putchar('\n');

						while (ptr < MAX_LEN_LINE) {
							line[cnt_sym_line++] = line[ptr];
							putchar(line[ptr++]);
						}
					}
				} else {
					cnt_sym_line = 0;
					putchar('\n');
				}
			}
			if (iscntrl(c)) {
				putchar('\a');
			} else {
				putchar(c);
				line[cnt_sym_line++] = c;
			}
			break;
		}

		fflush(stdout);
	}

	return 0;
}