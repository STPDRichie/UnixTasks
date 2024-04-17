#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

const int LOCK_TIME = 1;

const int PID_SIZE = sizeof(pid_t);

const int MAX_FILENAME_LENGTH = 255*8;

int file_exists(char* filename) {
    return access(filename, F_OK) == 0;
}

void main(int argc, char** argv) {
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

    while (1) {
        if (file_exists(lck_file_name)) {
            sleep(0.01);
            // printf("Файл %s уже используется другим процессом. Ожидание...\n", file_name);
            continue;
        }

        int lck_file = open(lck_file_name, O_WRONLY | O_CREAT, 0666);
        write(lck_file, str_pid, strlen(str_pid));
        close(lck_file);
        printf("Файл %s заблокирован для использования\n", file_name);

        sleep(LOCK_TIME); // Какие-то действия с файлом

        char* buff = (char*)calloc(100, PID_SIZE);

        if (!file_exists(lck_file_name)) {
            printf("Произошла ошибка\n");
            return;
        }

        lck_file = open(lck_file_name, O_RDONLY, 0666);

        int file_pid_index = read(lck_file, buff, 10);
        buff[file_pid_index] = '\0';

        if (strcmp(buff, str_pid) != 0) {
            printf("Произошла ошибка\n");
            close(lck_file);
            return;
        }

        close(lck_file);
        remove(lck_file_name);
        printf("Файл %s свободен для использования\n", file_name);
    }
}
