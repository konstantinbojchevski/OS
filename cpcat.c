#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

    int f1, f2;
    char *buffer = (char*) calloc(100, sizeof(char));
    int n1;

    if(argc == 2) {
        f1 = open(argv[1], O_RDONLY | O_EXCL);
        if(f1 < 0) {
            int koda = errno;
            perror("Cannot open file");
            exit(koda); 
        }'['
        f2 = 1;
    }

    else if(argc == 1) {
        f1 = 0;
        f2 = 1;
    }

    else {

        if(strcmp(argv[1], "-") == 0) {
            f1 = 0;
            f2 = open(argv[2], O_WRONLY | O_EXCL | O_TRUNC);
                if(f2 < 0) {
                    int koda = errno;
                    perror("Cannot open file");
                    exit(koda);
                }
        }
        else {
            f1 = open(argv[1], O_RDONLY | O_EXCL);
            if(f1 < 0) {
                int koda = errno;
                perror("Cannot open file");
                exit(koda);
            }
            f2 = open(argv[2], O_WRONLY | O_TRUNC | O_EXCL);
            if(f2 < 0) {
                int koda = errno;
                perror("Cannot open file");
                exit(koda);
            }
        }
    }

    while((n1 = read(f1, buffer, 10)) > 0) {
        if(write(f2, buffer, n1) != n1) {
            int koda = errno;
            perror("Cannot write to the file");
            exit(koda);
        }
    }
    exit(0);
}