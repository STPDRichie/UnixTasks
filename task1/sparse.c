#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void sparse(char* in_file_name, char* out_file_name, int block_size) {
    int in_file_read;
    if (in_file_name == NULL) {
        in_file_read = fileno(stdin);
    } else {
        in_file_read = open(in_file_name, O_RDONLY);
    }
    int out_file_write = open(out_file_name, O_WRONLY | O_CREAT, 0666);

    char buffer[block_size];
    int bytes_read;

    while ((bytes_read = read(in_file_read, buffer, block_size)) > 0) {
        int is_zero_block = 1;

        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] != 0) {
                is_zero_block = 0;
                break;
            }
        }

        if (!is_zero_block) {
            write(out_file_write, buffer, block_size);
        } else {
            lseek(out_file_write, block_size, SEEK_CUR);
        }
    }

    close(in_file_read);
    close(out_file_write);
}

void main(int argc, char** argv) {
    int block_size = 4096;

    char* in_file_name = NULL;
    char* out_file_name;
    
    if (argc < 2 || argc > 4) {
        printf("Передано неверное количество параметров: %d\n", argc - 1);
        return;
    }

    switch (argc) {
    case 2: {
        out_file_name = argv[1];
        break;
    }
    case 3: {
        in_file_name = argv[1];
        out_file_name = argv[2];
        break;
    }
    case 4: {
        in_file_name = argv[1];
        out_file_name = argv[2];
        char* endptr;
        block_size = strtol(argv[3], &endptr, 10);
        if (*endptr != '\0') {
            printf("Передано неверное значение параметра размера блока: %s, требуется число\n", argv[3]);
            return;
        }
        break;
    }
    default:
        break;
    }

    sparse(in_file_name, out_file_name, block_size);
}
