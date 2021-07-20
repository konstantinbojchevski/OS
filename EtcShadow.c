#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main() {

    int f1, f2 = 1;
    char *buffer = (char*) calloc(100, sizeof(char));
    int n1;
    char *file = "/etc/shadow";

    f1 = open(file, O_RDONLY | O_EXCL);
        if(f1 < 0) {
            int koda = errno;
            perror("Cannot open file");
            exit(koda);
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