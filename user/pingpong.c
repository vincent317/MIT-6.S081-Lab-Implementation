#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char a = 'a';
    int c_to_p[2];
    int p_to_c[2];
    char buf[1];
    pipe(c_to_p);
    pipe(p_to_c);

    if (fork() == 0) {
        // child
        close(p_to_c[1]);
        read(p_to_c[0], buf, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(c_to_p[1], &a, 1);
        exit(0);
    } else {
        // parent
        write(p_to_c[1],&a, 1);
        close(p_to_c[1]);
        close(c_to_p[1]);
        read(c_to_p[0], buf, 1);
        fprintf(1, "%d: received pong\n", getpid());
        exit(0);
    }
}