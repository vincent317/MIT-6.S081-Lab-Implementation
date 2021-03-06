#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(2, "sleep needs an argument\n", 24);
        exit(1);
    }
    int secs = atoi(argv[1]);

    sleep(secs);
    exit(0);
}