#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);
int str_to_int(char* str);

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char output[BUF_SIZE];
    char input[BUF_SIZE];
    int str_len = 0;
    int idx = 0, read_len = 0;
    int waitIndex;

    int operands_cnt = 0;
    int operand_idx;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    } else {
        puts("Connected...");
    }

    fputs("insert the count of operands: ", stdout);
    fgets(output, BUF_SIZE, stdin);
    
    output[strlen(output) - 1] = ',';

    fputs("insert operands(separeted by comma): ", stdout);
    fgets(output + strlen(output), BUF_SIZE, stdin);

    output[strlen(output) - 1] = ',';

    fputs("insert operator('+', '-', '*', '/' only): ", stdout);
    fgets(output + strlen(output), BUF_SIZE, stdin);

    output[strlen(output) - 1] = '\0';

    write(sock, output, BUF_SIZE);

    str_len = read(sock, input, BUF_SIZE - 1);
    output[str_len] = 0;

    printf("Answer: %s\n", input);

    close(sock);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int str_to_int(char* str) {
    int res = 0;
    char* cur_char = str;

    while (cur_char != NULL) {
        res *= 10;
        res += ((*cur_char) - '0');
    }

    return res;
}
