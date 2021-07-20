#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <dirent.h>

    gid_t getpid(void);
    gid_t getuid(void);
    gid_t geteuid(void);
    gid_t getgid(void);
    gid_t getegid(void);
    char *getcwd(char *buffer, size_t size);
    int getpriority(int which, id_t who);
    int access(const char *pathname, int how);
    int gettimeofday(struct timeval *tv, struct timezone *tz);
    int getrlimit(int resource, struct rlimit *rlim);

    void swap(int* a, int* b) {
        
        int temp = *a;
        *a = *b;
        *b = temp;
    }
 
    void selectionSort(int arr[], int n) {
    
        int i, j, min;
        for (i = 0; i < n - 1; i++) {
            min = i;
            for (j = i + 1; j < n; j++)
                if (arr[j] < arr[min])
                    min = j;
            swap(&arr[min], &arr[i]);
        }

    }
 
    struct PID_NAME {
        int process_pid;
        char *process_name;
    };

    struct PID_NAME_CND_PPID {
        int process_pid;
        char *process_name;
        char *condition;
        int process_ppid;
    };

    struct PID_NAME_CND_PPID_TDS_NOF {
        int process_pid;
        char *process_name;
        char *condition;
        int process_ppid;
        int threads;
        int num_open_files;
    };

int main(int argc, char* argv[]) {

    if(strcmp(argv[1], "sys") == 0) {
        
        FILE *proc_version;
        char c;
        int blank = 0;
        proc_version = fopen(strcat(argv[2], "/version"), "r");
        
        while(1) {

            c = fgetc(proc_version);
            if(feof(proc_version)) {
                break;
            }

            if(c == ' ') {
                if(blank == 0 || blank == 4) 
                    printf(":");
                if(blank == 2 || blank == 6)
                    printf("\n");
                blank++;
            } 
            
            if(blank == 0) {
                printf("%c", c);
            }

            if(blank == 2) {
                printf("%c", c);
            }
            

            if(blank == 4) {
                if(!(c == ' ' || c == '('))
                   printf("%c", c);
            }

            if(blank == 6){
                printf("%c", c);
            }

        }

        fclose(proc_version);

    }

    if(strcmp(argv[1], "sysext") == 0) {

        FILE *proc_version;
        char version_file[256];
        strcat(strcpy(version_file, argv[2]), "/version");

        FILE *swaps;
        char swaps_file[256];
        strcat(strcpy(swaps_file, argv[2]), "/swaps");

        FILE *modules;
        char modules_file[256];
        strcat(strcpy(modules_file, argv[2]), "/modules");

        char c;
        int blank = 0;
        proc_version = fopen(version_file, "r");
        
        char version[256];
        char gcc_ve[256];

        fscanf(proc_version, "%s", version);
        printf("%s: ", version);
        fscanf(proc_version, "%s", version);
        fscanf(proc_version, "%s", version);
        printf("%s\n", version);


        for(int j=0; j<4; j++) {
            fscanf(proc_version, "%s", gcc_ve);
        }
        printf("gcc: %s\n", gcc_ve);
        
        char line[2048];
        swaps = fopen(swaps_file, "r");

        int i = 0;
        while (i < 2) {
            fgets(line, sizeof(line), swaps);
            i++;
        }

        char *ptr = strtok(line, " ");
        printf("Swap: %s\n", ptr);

        fclose(swaps);

        modules = fopen(modules_file, "r");

        i = 0;
        while (fgets(line, sizeof(line), swaps)) {
            i++;
        }

        printf("Modules: %d\n", i);

        fclose(modules);

        exit(i);

    }

    if(strcmp(argv[1], "me") == 0) {
        
        struct utsname buffer;
        char cwd[256];
        int which = PRIO_PROCESS;
        char str_pid[256];
        char proc_path[256];
        struct timeval tv;
        struct timezone tz;
        struct rlimit cpu_lim;

        errno = 0;
        if (uname(&buffer) < 0) {
            perror("uname");
            exit(EXIT_FAILURE);
        }

        printf("Uid: %d\n", getuid());
        printf("EUid: %d\n", geteuid());
        printf("Gid: %d\n", getgid());
        printf("EGid: %d\n", getegid());
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Cwd: %s\n", cwd);
        }

        printf("Priority: %d\n", getpriority(which, getpid()));
        
        sprintf(str_pid, "%d", getpid());
        strcat(proc_path, "/proc/");
        strcat(proc_path, str_pid);
        strcat(proc_path, "/");
        printf("Process proc path: %s\n", proc_path);
        if (access(proc_path, F_OK) == 0)
            printf("Process proc access: yes\n");

        printf("OS name: %s\n", buffer.sysname);
        printf("OS release: %s\n", buffer.release);
        printf("OS version: %s\n", buffer.version);
        printf("Machine: %s\n", buffer.machine);
        printf("Node name: %s\n", buffer.nodename);

        gettimeofday(&tv, &tz);
        printf("Timezone: %d\n", tz.tz_dsttime);
        if( getrlimit(RLIMIT_CPU, &cpu_lim) == 0) {
            printf("CPU limit: %lu\n", cpu_lim.rlim_max);
        }

    }

    if(strcmp(argv[1], "pids") == 0) {

        DIR *dir;
        struct dirent *folder;
        dir = opendir(argv[2]);
        int *arr = (int*)calloc(0, sizeof(int));
        int s = 0;
        int i = 0;

        while((folder = readdir(dir)) != NULL) {
            
            DIR *in;
            char p[512];
            strcat(strcpy(p, argv[2]), "/");
            if(!(strcmp(folder->d_name, ".") == 0 || strcmp(folder->d_name, "..") == 0)) in = opendir(strcat(p, folder->d_name));
            else in = NULL;
            
            if(in != NULL) {
                FILE *stat;
                stat = fopen(strcat(p, "/stat"), "r");
                if(stat != NULL) {

                    char line[1024];
                    fgets(line, 1024, stat);

                    char *ptr = strtok(line, " ");
                    int num = atoi(ptr);
                    s++;
                    arr = realloc(arr, s * sizeof(int));
                    arr[i] = num;
                    i++;
                    
                }
                fclose(stat);
            
            }
            closedir(in);

        }

        selectionSort(arr, s);
        for (i = 0; i < s; ++i) {
            printf("%d\n", arr[i]);
        }

        closedir(dir);
    
    }

    if(strcmp(argv[1], "names") == 0) {

        DIR *dir;
        struct dirent *folder;
        dir = opendir(argv[2]);
        struct PID_NAME **arr = NULL;
        char name[256];
        int pid;
        int s = 0;
        int i = 0;

        while((folder = readdir(dir)) != NULL) {

            DIR *in;
            char p[512];
            strcat(strcpy(p, argv[2]), "/");
            if(!(strcmp(folder->d_name, ".") == 0 || strcmp(folder->d_name, "..") == 0)) in = opendir(strcat(p, folder->d_name));
            else in = NULL;
            
            if(in != NULL) {
                FILE *stat;
                stat = fopen(strcat(p, "/stat"), "r");

                if(stat != NULL) {
                    fscanf(stat, "%d %s", &pid, name);
                    char *real_process_name = name;
                    real_process_name++;
                    real_process_name[strlen(real_process_name) - 1] = 0;
                    s++;

                    arr = (struct PID_NAME**) realloc(arr, s * sizeof(struct PID_NAME*));
                    arr[i] = (struct PID_NAME*) malloc(sizeof(struct PID_NAME));
                    arr[i]->process_pid = pid;
                    arr[i]->process_name = strdup(real_process_name);
                    i++;
                }
                fclose(stat);
            
            }
            closedir(in);

        }

        for(int j=0; j<s; j++) {

            for(int k=j+1; k<s; k++) {

                if(strcasecmp(arr[j]->process_name, arr[k]->process_name) == 0) {

                    if(arr[j]->process_pid > arr[k]->process_pid) {
                        struct PID_NAME *tmp = arr[j];
                        arr[j] = arr[k];
                        arr[k] = tmp;
                    }

                }

                else {

                    if(strcasecmp(arr[j]->process_name, arr[k]->process_name) > 0) {
                        struct PID_NAME *tmp = arr[j];
                        arr[j] = arr[k];
                        arr[k] = tmp;
                    }
                    
                }

            }

        }

        for(int j=0; j<s; j++) {

            printf("%d %s\n", arr[j]->process_pid, arr[j]->process_name);

        }

        for(int j=0; j<s; j++) {

            free(arr[j]->process_name); free(arr[j]);

        }       

        closedir(dir);
    
    }

    if(strcmp(argv[1], "ps") == 0) {

         DIR *dir;
        struct dirent *folder;
        dir = opendir(argv[2]);
        struct PID_NAME_CND_PPID **arr = NULL;
        char name[256];
        int pid;
        int ppid;
        char stanje[256];
        int s = 0;
        int i = 0;
        
        while((folder = readdir(dir)) != NULL) {

            DIR *in;
            char p[512];
            strcat(strcpy(p, argv[2]), "/");
            if(!(strcmp(folder->d_name, ".") == 0 || strcmp(folder->d_name, "..") == 0)) in = opendir(strcat(p, folder->d_name));
            else in = NULL;
            
            if(in != NULL) {
                FILE *stat;
                stat = fopen(strcat(p, "/stat"), "r");

                if(stat != NULL) {
                    fscanf(stat, "%d %s %s %d", &pid, name, stanje, &ppid);
                    char *real_process_name = name;
                    real_process_name++;
                    real_process_name[strlen(real_process_name) - 1] = 0;
                    s++;
                    
                    arr = (struct PID_NAME_CND_PPID**) realloc(arr, s * sizeof(struct PID_NAME_CND_PPID*));
                    arr[i] = (struct PID_NAME_CND_PPID*) malloc(sizeof(struct PID_NAME_CND_PPID));
                    arr[i]->process_pid = pid;
                    arr[i]->process_name = strdup(real_process_name);
                    arr[i]->condition = strdup(stanje);
                    arr[i]->process_ppid = ppid;
                    i++;
                }
                fclose(stat);
            
            }
            closedir(in);

        }


        for(int j=0; j<s; j++) {

            for(int k=j+1; k<s; k++) {

                if(arr[j]->process_pid > arr[k]->process_pid) {
                    struct PID_NAME_CND_PPID *tmp = arr[j];
                    arr[j] = arr[k];
                    arr[k] = tmp;
                }

            }

        }

        if(argc == 4) {

            printf("%5s %5s %6s %s\n", "PID", "PPID", "STANJE", "IME");
            int c = atoi(argv[3]);

            int *barani = (int*) calloc(s, sizeof(int));
            barani[0] = c;
            int b=1;

            for(int i=0; i<sizeof(barani); i++) {
                if(barani[i] == 0) break;
                for(int j=0; j<s; j++) {
                    if(barani[i] == arr[j]->process_ppid) {
                        barani[b] = arr[j]->process_pid;
                        b++;
                    }
                }
            }
            
            for(int j=0; j<s; j++) {
    
                for(int k=0; k<s; k++) {

                    if(arr[j]->process_pid == barani[k])
                        printf("%5d %5d %6s %s\n", arr[j]->process_pid, arr[j]->process_ppid, arr[j]->condition, arr[j]->process_name);
                
                }
            
            }
           
        }
        else {

            printf("%5s %5s %6s %s\n", "PID", "PPID", "STANJE", "IME");
            for(int j=0; j<s; j++) {

                printf("%5d %5d %6s %s\n", arr[j]->process_pid, arr[j]->process_ppid, arr[j]->condition, arr[j]->process_name);

            }

        }
 
        for(int j=0; j<s; j++) {

            free(arr[j]->process_name); 
            free(arr[j]->condition);
            free(arr[j]);

        }       

        closedir(dir);
    
    }

    if(strcmp(argv[1], "psext") == 0) {

        DIR *dir;
        struct dirent *folder;
        dir = opendir(argv[2]);
        struct PID_NAME_CND_PPID_TDS_NOF **arr = NULL;
        char name[256];
        int pid;
        int ppid;
        char stanje[256];
        int niti;
        int s = 0;
        int i = 0;
        
        while((folder = readdir(dir)) != NULL) {

            DIR *in;
            char p[512];
            strcat(strcpy(p, argv[2]), "/");
            if(!(strcmp(folder->d_name, ".") == 0 || strcmp(folder->d_name, "..") == 0)) in = opendir(strcat(p, folder->d_name));
            else in = NULL;
            
            if(in != NULL) {
                FILE *stat;
                DIR *fd;
                char fd_descriptor[256];
                strcpy(fd_descriptor, p);
                fd = opendir(strcat(fd_descriptor, "/fd"));
                struct dirent *fd_folder;
                int n_o_d = 0;

                if(fd != NULL) {

                    while((fd_folder = readdir(fd)) != NULL) {
                        n_o_d++;    
                    }

                }
                closedir(fd);
                
                stat = fopen(strcat(p, "/stat"), "r");

                if(stat != NULL) {

                    fscanf(stat, "%d %s %s %d", &pid, name, stanje, &ppid);
                    for(int j=0; j<16; j++) {
                        fscanf(stat, "%d", &niti);
                    }
                    
                    char *real_process_name = name;
                    real_process_name++;
                    real_process_name[strlen(real_process_name) - 1] = 0;
                    s++;
                    
                    arr = (struct PID_NAME_CND_PPID_TDS_NOF**) realloc(arr, s * sizeof(struct PID_NAME_CND_PPID_TDS_NOF*));
                    arr[i] = (struct PID_NAME_CND_PPID_TDS_NOF*) malloc(sizeof(struct PID_NAME_CND_PPID_TDS_NOF));
                    arr[i]->process_pid = pid;
                    arr[i]->process_name = strdup(real_process_name);
                    arr[i]->condition = strdup(stanje);
                    arr[i]->process_ppid = ppid;
                    arr[i]->threads = niti;
                    arr[i]->num_open_files = n_o_d - 2;
                    i++;
                }
                fclose(stat);
            
            }
            closedir(in);

        }

        for(int j=0; j<s; j++) {

            for(int k=j+1; k<s; k++) {

                if(arr[j]->process_pid > arr[k]->process_pid) {
                    struct PID_NAME_CND_PPID_TDS_NOF *tmp = arr[j];
                    arr[j] = arr[k];
                    arr[k] = tmp;
                }

            }

        }

        if(argc == 4) {

            printf("%5s %5s %6s %6s %6s %s\n", "PID", "PPID", "STANJE", "#NITI", "#DAT.", "IME");
            int c = atoi(argv[3]);

            int *barani = (int*) calloc(s, sizeof(int));
            barani[0] = c;
            int b=1;

            for(int i=0; i<sizeof(barani); i++) {
                if(barani[i] == 0) break;
                for(int j=0; j<s; j++) {
                    if(barani[i] == arr[j]->process_ppid) {
                        barani[b] = arr[j]->process_pid;
                        b++;
                    }
                }
            }
            
            for(int j=0; j<s; j++) {
    
                for(int k=0; k<s; k++) {

                    if(arr[j]->process_pid == barani[k])
                        printf("%5d %5d %6s %6d %6d %s\n", arr[j]->process_pid, arr[j]->process_ppid, arr[j]->condition, arr[j]->threads, arr[j]->num_open_files, arr[j]->process_name);

                }
            
            }               
           
        }
        else {

            printf("%5s %5s %6s %6s %6s %s\n", "PID", "PPID", "STANJE", "#NITI", "#DAT.", "IME");
            for(int j=0; j<s; j++) {

                printf("%5d %5d %6s %6d %6d %s\n", arr[j]->process_pid, arr[j]->process_ppid, arr[j]->condition, arr[j]->threads, arr[j]->num_open_files, arr[j]->process_name);

            }

        }
 
        for(int j=0; j<s; j++) {

            free(arr[j]->process_name); 
            free(arr[j]->condition);
            free(arr[j]);

        }       

        closedir(dir);
    
    }

}