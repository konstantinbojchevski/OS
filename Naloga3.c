#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <ctype.h>

int tokenize(char* line);
char tokens[30][256];
int token_count = 0;
char ime[5] = "mysh";
int status = 0;
void vorname();
void printaj();
void printaj_2();
void print_pid();
void print_ppid();
void print_status();
void print_exit();
void dirchange();
void dirwhere();
void dirmake();
void dirremove();
void dirlist();
void dirrename();

int main() {

	bool repl = false;
	while(!repl) {
        size_t size;
        char *line = NULL;
        //printf("%s> ", ime);
        
        if (getline(&line, &size, stdin) != -1) {        
            char trenutna[256];
            strcpy(trenutna, line);
            trenutna[strlen(trenutna)-1] = '\0';
            token_count = tokenize(trenutna);
                
                for(int i=0; i<token_count; i++) {
                    if(i == 0 && strcmp(tokens[0], "name") == 0) {
                        vorname();
                    }
                    if(i == 0 && strcmp(tokens[0], "print") == 0) {
                        printaj();
                    }
                    if(i == 0 && strcmp(tokens[0], "echo") == 0) {
                        printaj_2();
                    }
                    if(i == 0 && strcmp(tokens[0], "status") == 0) {
                        print_status();
                    }
                    if(i == 0 && strcmp(tokens[0], "exit") == 0) {
                        print_exit();
                    }
                    if(i == 0 && strcmp(tokens[0], "pid") == 0) {
                        print_pid();
                    }
                    if(i == 0 && strcmp(tokens[0], "ppid") == 0) {
                        print_ppid();
                    }
                    if(i == 0 && strcmp(tokens[0], "dirchange") == 0) {
                        dirchange();
                    }
                    if(i == 0 && strcmp(tokens[0], "dirwhere") == 0) {
                        dirwhere();
                    }
                    if(i == 0 && strcmp(tokens[0], "dirmake") == 0) {
                        dirmake();
                    }
                    if(i == 0 && strcmp(tokens[0], "dirremove") == 0) {
                        dirremove();
                    }
                    if(i == 0 && strcmp(tokens[0], "dirlist") == 0) {
                        dirlist();
                    }
                    if(i == 0 && strcmp(tokens[0], "rename") == 0) {
                        dirrename();
                    }
                    fflush(stdout);
                }		          
        }
		else repl = true;
    }
    return 0;
}

void vorname() {
    status = 0;
    if(token_count != 0) {
        if(token_count == 1) {
            printf("%s\n", ime);
        }
        else {
            strcpy(ime, tokens[1]);
        }
    }
}

void printaj() {
    status = 0;
    if(token_count != 0) {
        for(int i=1; i<token_count; i++) {
            char zbor[60];
            strcpy(zbor, tokens[i]);
            if(i != token_count-1)
                printf("%s ", zbor);
            else
                printf("%s", zbor);
        }
    }
}

int tokenize(char* line) {
 
    char* c = line;
    while(*c == ' ') c++;
 
    c = line;
    int indeks_red = 0;
    int indeks_bukva = 0;
    
    while(*c != '\0') {

        while(*c == ' ') c++;
        if(*c == '#') break;
        
        if(*c == '\"'){                        
            c++;
            while(*c != '\0' && *c != '\"') {
                tokens[indeks_red][indeks_bukva] = *c;
                indeks_bukva++;
                c++;
            }
            tokens[indeks_red][indeks_bukva] = '\0';
            indeks_red++;
            indeks_bukva = 0;
            c++;
        }
        
        if(*c != ' ' && *c != '\0') {
            while(*c != '\0' && *c != ' ') {
                tokens[indeks_red][indeks_bukva] = *c;
                indeks_bukva++;
                c++;
            }
            tokens[indeks_red][indeks_bukva] = '\0';
            indeks_red++;                                    
            indeks_bukva = 0;
        }
        c++;
    }
    return indeks_red;
}

void printaj_2() {
    status = 0;
    for(int i=1; i<token_count; i++) {
            char zbor[60];
            strcpy(zbor, tokens[i]);
            char *p = zbor;
            char c = p[0];
            char s = p[strlen(p)-1];
            if(c == '"' && s == '"') {
                p++;
                p[strlen(p) - 1] = 0;
            }
            printf("%s ", p);
    }
    printf("\n");
}

void print_pid() {
    status = 0;
    printf("%d\n", getpid());
}

void print_ppid() {
    status = 0;
    printf("%d\n", getppid());
}

void print_status() {
    printf("%d\n", status);
}

void print_exit() {
    status = 0;
    int stat = atoi(tokens[1]);
    exit(stat);
}

void dirchange() {
    status = 0;
    if(token_count > 1) {
        char to[30];
        strcpy(to, tokens[1]);
        if(chdir(to) != 0) {
            int koda = errno;
            perror("dirchange");
            status = koda;
        }
    }
    else {
        if(chdir("/") != 0) {
            int koda = errno;
            perror("dirchange");
            status = koda;
        }
    }
}

void dirwhere() {
    char current[256];
    status = 0;
    if(getcwd(current, sizeof(current)) != NULL) {
        printf("%s\n", current);
    }
    else {
        int koda = errno;
        perror("getcwd()");
        status = koda;
    }
}

void dirmake() {
    char dir[256];
    strcpy(dir, tokens[1]);
    status = 0;
    if(mkdir(dir, 0777) != 0) {
        int koda = errno;
        perror("dirmake");
        status = koda;  
    }
}

void dirremove() {
    char dir[256];
    strcpy(dir, tokens[1]);
    status = 0;
    if (rmdir(dir) != 0) {
        int koda = errno;
        perror("dirremove");
        status = koda;
    }
}

void dirlist() {
    DIR *d;
    struct dirent *directory; 
    char dir[256];
    status = 0;
    if(token_count > 1) {
        strcpy(dir, tokens[1]);
    }
    else {
        strcpy(dir, "./");
    }
    d = opendir (dir);

    if (d != NULL)
    {
        while (directory = readdir(d))
        printf("%s ", directory->d_name);
    }
    printf("\n");
}

void dirrename() {
    char from[256];
    char to[256];
    strcpy(from, tokens[1]);
    strcpy(to, tokens[2]);
    status = 0;
    if(rename(from, to) != 0) {
        status = errno;
    }
}