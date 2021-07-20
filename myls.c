#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

int main(int argc, char *argv[]) {

    char *directory;

    if(argc == 1) directory = "./";
    else directory = argv[1];

    DIR *dir;
    struct dirent *d;

    if((dir = opendir(directory)) == NULL) {
        int koda = errno;
        perror("Cannot open directory");
        exit(koda);
    }

    int koda = errno;
    koda = 0;
    while ((d = readdir(dir)) != NULL) {
       printf("%s ", d->d_name);
    }
    if(koda != 0) {
        perror("Cannot read directory");
    }
    printf(" \n");

    if(closedir(dir) < 0) {
        int koda = errno;
        perror("Cannot close directory");
        exit(koda);
    }
}



