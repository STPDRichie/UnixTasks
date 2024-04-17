#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

const int LOCK_TIME = 1;

const int PID_SIZE = sizeof(pid_t);

const int MAX_FILENAME_LENGTH = 255*8;

int locks_count = 0;

char* get_lck_file_str_pid(char* lck_file_name) {
    int lck_file = open(lck_file_name, O_RDONLY, 0666);

    char* buff = (char*)calloc(100, PID_SIZE);
    int file_pid_index = read(lck_file, buff, 10);
    buff[file_pid_index] = '\0';

    return buff;
}

void sigint_handler(int signum, char* file_name) {
    static char* lck_file_name = NULL;

    if (lck_file_name == NULL) {
        lck_file_name = file_name;
    }
    int pid = (int)getpid();
    char str_pid[PID_SIZE];
    sprintf(str_pid, "%d", pid);

    if (signum == -1) {
        return;
    }

    char* lck_file_pid = get_lck_file_str_pid(lck_file_name);
    if (strcmp(lck_file_pid, str_pid) == 0) {
        remove(lck_file_name);
    }

    FILE* result_file = fopen("result.txt", "a");
    fprintf(result_file, "[pid:%d] всего блокировок - %d\n", pid, locks_count);
    fclose(result_file);
    exit(0);
}

int file_exists(char* filename) {
    return access(filename, F_OK) == 0;
}

void main(int argc, char** argv) {
    signal(SIGINT, (void (*)(int))sigint_handler);

    if (argc != 2) {
        printf("Передано неверное количество параметров");
        return;
    }

    int pid = (int)getpid();

    char str_pid[PID_SIZE];
    sprintf(str_pid, "%d", pid);

    char* file_name = argv[1];

    char lck_file_name[MAX_FILENAME_LENGTH];
    sprintf(lck_file_name, "%s.lck", file_name);

    sigint_handler(-1, lck_file_name);

    while (1) {
        if (file_exists(lck_file_name)) {
            sleep(0.01);
            continue;
        }

        int lck_file = open(lck_file_name, O_WRONLY | O_CREAT, 0666);
        write(lck_file, str_pid, strlen(str_pid));
        close(lck_file);
        printf("Файл %s заблокирован для использования\n", file_name);
        locks_count += 1;

        sleep(LOCK_TIME); // Какие-то действия с файлом

        if (!file_exists(lck_file_name)) {
            printf("Произошла ошибка\n");
            return;
        }

        char* lck_file_pid = get_lck_file_str_pid(lck_file_name);

        if (strcmp(lck_file_pid, str_pid) != 0) {
            printf("Произошла ошибка\n");
            close(lck_file);
            return;
        }

        close(lck_file);
        remove(lck_file_name);
        printf("Файл %s свободен для использования\n", file_name);
    }
}
