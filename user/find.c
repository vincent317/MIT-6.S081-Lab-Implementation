#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char *path, char *target) {
    int fd;
    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    char buf[512], *p;
    struct dirent de;
    struct stat st;

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if (de.inum == 0) {
            continue;
        }
        
        char rel_name[15] = {0};
        memmove(rel_name, de.name, DIRSIZ);
        if (strcmp(rel_name, ".") == 0 || strcmp(rel_name, "..") == 0) {
            continue;
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        

        if(stat(buf, &st) < 0){
            printf(buf);
            printf("ls: cannot stat %s\n", buf);
            continue;
        }

        if (st.type == 1) {
            // directory
            find(buf, target);
        } else if (st.type == 2) {
            // regular file
            if (strcmp(rel_name, target) == 0) {
                printf(buf);
                printf("\n");
            }
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    find(argv[1], argv[2]);
    exit(0);
}