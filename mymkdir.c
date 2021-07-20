#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {

    if(argc == 1) {
        write(1, "Podajte ime imenika kot prvi argument\n", 40);
    }
    else {
        if(mkdir(argv[1], 0777) < 0) {
            int koda = errno;
            perror("Cannot create file");
            exit(koda);
        }
    }
}