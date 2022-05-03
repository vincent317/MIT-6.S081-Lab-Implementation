#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int *fd_handler(int *left_fd) {
    close(left_fd[1]);

    int left;
    read(left_fd[0], &left, sizeof(int));
    fprintf(1, "prime %d\n", left);

    int *right_fd = malloc(sizeof(int) * 2);
    pipe(right_fd);
    return right_fd;
}

void sieve(int flag, int *leftfd, int *rightfd) {
    int num2;
    close(rightfd[0]);
    while (read(leftfd[0], &num2, sizeof(int)) != 0) {
        if (num2 % flag !=0) {
            write(rightfd[1], &num2, sizeof(int));
        }
    }
    close(rightfd[1]);
    close(leftfd[0]);
    int waitflag = 0;
    wait(&waitflag);
    free(rightfd);
    exit(0);
}

int main(int argc, char *argv[]) {
    int feed_fd[2];
    pipe(feed_fd);
    
    if (fork() != 0) {
        int i;
        // process which feeds 2-35 into the pipeline
        close(feed_fd[0]);
        for (i=2; i<36;i ++) {
            write(feed_fd[1], &i, sizeof(int));
        }
        close(feed_fd[1]);

        int waitflag = 0;
        wait(&waitflag);
        
        exit(0);
    } else {
        // process that sieves 2
        int *two_right = fd_handler(feed_fd);
        
        if (fork() != 0) {
            sieve(2, feed_fd, two_right);
        } else {
            // process that sieves 3
            int *three_right = fd_handler(two_right);
           
            if (fork() != 0) {
                sieve(3, two_right, three_right);
            } else {
                // process that sieves 5
                int *five_right = fd_handler(three_right);

                if (fork() != 0) {
                    sieve(5, three_right, five_right);
                } else {
                    // process that sieves 7
                    int *seven_right = fd_handler(five_right);

                    if (fork() != 0) {
                        sieve(7, five_right, seven_right);
                    } else {
                        // process that sieves 11
                        int *eleven_right = fd_handler(seven_right);

                        if (fork() != 0) {
                            sieve(11, seven_right, eleven_right);
                        } else {
                            // process that sieves 13
                            int *thirteen_right = fd_handler(eleven_right);

                            if (fork() != 0) {
                                sieve(13, eleven_right, thirteen_right);
                            } else {
                                // process that sieves 17
                                int *seventeen_right = fd_handler(thirteen_right);

                                if (fork() != 0) {
                                    sieve(17, thirteen_right, seventeen_right);
                                } else {
                                    // process that sieves 19
                                    int *nineteen_right = fd_handler(seventeen_right);

                                    if (fork() != 0) {
                                        sieve(19, seventeen_right, nineteen_right);
                                    } else {
                                        // process that sieves 23
                                        int *twentythree_right = fd_handler(nineteen_right);

                                        if (fork() != 0) {
                                            sieve(23, nineteen_right, twentythree_right);
                                        } else {
                                            // process that sieves 29
                                            int *twentynine_right = fd_handler(twentythree_right);

                                            if (fork() != 0) {
                                                sieve(29, twentythree_right, twentynine_right);
                                            } else {
                                                /* 
                                                    process that sieves 31
                                                    cleans the pipe
                                                */
                                                close(twentynine_right[1]);

                                                int left;
                                                read(twentynine_right[0], &left, sizeof(int));
                                                fprintf(1, "prime %d\n", left);

                                                while (read(twentynine_right[0], &left, sizeof(int)) != 0) {}
                                                close(twentynine_right[0]);
                                                exit(0);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
           }
        }
    }
    return 0;
}