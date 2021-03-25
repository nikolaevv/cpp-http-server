#include <memory>
#include <cstdint>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/epoll.h>

using std::cout;
using std::string;
using std::uint16_t;
using std::unique_ptr;

void * handle_connection(int);

const int BUFSIZE = 4096;
const int PATH_MAX = 1;

int main () {
    //char const server_adress [] = "127.0.0.1";
    //uint16_t server_port = 5555;

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    
    char sendBuff[1025];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // Создание в области ядра неименованного сетевого сокета с протоколом TCP
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    // Создание сокета с заданными адресом и портом

    listen(listenfd, 10);
    // Устанавливаем максимально допустимое кол-во подключений

    //int pollingfd = epoll_create(0xCAFE);
    // Создание дескриптора epoll для мониторинга сокетов

    //struct epoll_event ev = {0};
    // Инициализация структуры epoll-event

    fd_set current_sockets, ready_sockets;
    // Инициализация наборов сокетов
    FD_ZERO(&current_sockets);
    FD_SET(listenfd, &current_sockets);

    while (true) {
        // Ожидание входящего соединения

        // ready_sockets является буффером набора сокетов, т.к. select изменяет исходный набор
        ready_sockets = current_sockets;

        if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
            cout << "Ошибка select";
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &ready_sockets)) {
                if (i == listenfd ) {
                    // Новое соединение, которое необходимо принять
                    //connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
                    int client_socket = accept(listenfd, (struct sockaddr*)NULL, NULL);
                    FD_SET(client_socket, &current_sockets);
                    
                } else {
                    //handle_connection(i);
                }
            }
        }

        //connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        // Создание нового сокета для очередного соединения
                                                                                                                                                                              
        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n Hello fucking world", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));
        // Принт даты

        close(connfd);
        // Закрываем соединение
        sleep(1);
    }

    return 0;
}

void * handle_connection (int client_socket) {
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX + 1];

    while ((bytes_read = read(client_socket, buffer + msgsize, sizeof(buffer) - msgsize - 1))) {
        msgsize += bytes_read;

        if (msgsize > BUFSIZE - 1 || buffer[msgsize-1] == '\n') {
            // При заполнении буффера или ококнчании сообщения прекращаем чтение
            break;
        }

        //check(bytes_read, "recv error");
        buffer[msgsize - 1] = 0;

        printf("request %s\n", buffer)
        //fflush(stdout);

        //close_co
    }

}

int setup_server() {

}