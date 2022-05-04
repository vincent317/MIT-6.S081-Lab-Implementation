#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int read_line (int fd, char *buf) {
    char c;
    char *p = buf;

    while (1) {
        if (!read(fd, &c, 1)) {
            // all lines read
            return 0;
        }
        if (c == '\n') {
            // one line ended
            *p = 0;
            return 1;
        }
        *p++ = c;
    }
}

int main(int argc, char *argv[]) {
    char buf[512];
    char *args[MAXARG + 2];
    int i = 0;

    while (i < argc - 1) {
        args[i] = argv[i+1];
        i++;
    }

    while (read_line(0, buf)) {
        if (fork() == 0) {
            args[i] = buf;
            args[i+1] = 0;
           
            exec(args[0], args);
        } else {
            wait(0);
        }
    }

    exit(0);
    return 0;
}