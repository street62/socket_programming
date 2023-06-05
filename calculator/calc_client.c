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

    int output[BUF_SIZE] = {0, };
    int input[BUF_SIZE] = {0, };
    
    int idx, str_len;

    int operands_cnt;
    int cur_operand;
    char operator;

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
    scanf("%d", &operands_cnt);
    *output = operands_cnt;
 
    for (idx = 0; idx < operands_cnt; idx++) {
        printf("operand %d: ", idx + 1);
        scanf("%d", &cur_operand);
        *(output + idx + 1) = cur_operand;
    }

    fgetc(stdin);
    
    fputs("insert operator('+', '-', '*', '/' only): ", stdout);
    scanf("%c", &operator);

    *(output + operands_cnt + 1) = (int)operator;

    write(sock, output, sizeof(int) * (operands_cnt + 2));

    printf("calculating...\n");

    str_len = read(sock, input, BUF_SIZE - 1);

    printf("Answer: %d\n", input[0]);

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
