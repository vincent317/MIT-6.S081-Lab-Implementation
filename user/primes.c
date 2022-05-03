#include "kernel/types.h"
#include "user/user.h"

void prime(int *leftp) {
    close(leftp[1]);

    int first;
    int remain = read(leftp[0], &first, sizeof(int));
    if (remain == 0) {
        // this takes care of the process created by the last seen prime
        exit(0);
    }
    fprintf(1, "prime %d\n", first);

    int rightp[2];
    pipe(rightp);

    if (fork() == 0) {
        prime(rightp);
    } else {
        close(rightp[0]);
        int checking;

        while (read(leftp[0], &checking, sizeof(int)) != 0) {
            if (checking % first != 0) {
                write(rightp[1], &checking, sizeof(int));
            }
        }
        close(leftp[0]);
        close(rightp[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int feed_fd[2];
    pipe(feed_fd);
    
    if (fork() == 0) {
        prime(feed_fd);
    } else {
        // process which feeds 2-35 into the pipeline
        int i;
        close(feed_fd[0]);
        for (i=2; i<36; i ++) {
            write(feed_fd[1], &i, sizeof(int));
        }
        close(feed_fd[1]);

        wait(0);
        exit(0);
    }

    return 0;
}