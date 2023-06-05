#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);
int str_to_int(char* str);
int get_res(int operands[], int len, char operator);

int main(int argc, char* argv[]) {
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    int input[BUF_SIZE] = {0, };
    int output[BUF_SIZE] = {0, };

    int str_len;
    int i, j;
    int res;

    int operand_cnt;
    char operator;
    int operands[BUF_SIZE] = { 0, };

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (serv_sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    
    for (i = 0; i < 5; i++) {
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            error_handling("accept() error!");
        } else {
            printf("Conncected Client: %d\n", i + 1);
        }

        read(clnt_sock, input, sizeof(int));
        operand_cnt = input[0];

        printf("operand count : %d\n", operand_cnt);

        read(clnt_sock, input + 1, sizeof(int) * (operand_cnt + 1));

        for (j = 0; j < operand_cnt; j++) {
            operands[j] = input[1 + j];
            printf("oprand %d: %d\n", j + 1, operands[j]);
        }

        operator = input[operand_cnt + 1];
        res = get_res(operands, operand_cnt, operator);
        output[0] = res;

        write(clnt_sock, output, BUF_SIZE);

        close(clnt_sock);
    }

    close(serv_sock);

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

    while (*cur_char != 0) {
        res *= 10;
        res += ((*cur_char) - '0');
        cur_char += 1;
    }

    return res;
}

int get_res(int operands[], int len, char operator) {
    int res = operands[0];
    int i;

    printf("operator: %c\n", operator);
    printf("calculating...\n");

    switch (operator)
    {
    case '+':
        for (i = 1; i < len; i++) {
            res += operands[i];
        }
        break;
    case '-':
        for (i = 1; i < len; i++) {
            res -= operands[i];
        }
        break;
    case '*':
        for (i = 1; i < len; i++) {
            res *= operands[i];
        }
        break;
    case '/':
        for (i = 1; i < len; i++) {
            res /= operands[i];
        }
        break;
    default:
        fprintf(stderr, "get_res(): INVALID OPERATOR %c", operator);
        break;
    }

    printf("res: %d\n", res);

    return res;
}