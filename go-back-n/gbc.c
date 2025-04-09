#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>

struct timeval timeout;
char buff[200];

void func(int socket, int fram, int win){
    int start=0, stop=win, sent=0, items=0;
    if(setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&timeout, sizeof(timeout)) < 0){
        printf("setsockopt(SO_RCVTIMEO) failed.\n");
    }

    while(sent < fram){
        
        while(items < win && start < fram){
            bzero(buff, 200);
            snprintf(buff, sizeof(buff), "%d", start);
            send(socket, buff, sizeof(buff), 0);
            sleep(1);
            printf("Frame %d sent.\n", start);
            start++;
            items++;
        }
        recv(socket, buff, sizeof(buff), 0);
        int frame = atoi(buff);

        if(frame != sent){
            printf("Ack received for frame %d. Frame %d lost.\n", frame, sent);
            printf("Resending frames.\n");
            items = 0;
            start = sent;
            continue;
        }

        printf("Ack received for frame %d.\n", sent);
        items--;
        sent++;
    }
    snprintf(buff, sizeof(buff), "%d", -1);
    send(socket, buff, sizeof(buff), 0);
    printf("sent %s.\n", buff);
}

int main(void){
    int sockdesc;
    struct sockaddr_in server_addr;
    int fram, win;

    sockdesc = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if(sockdesc < 0){
        printf("COuldnt create socket.\n");
        return -1;
    }
    printf("Created socket.\n");

    if(connect(sockdesc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect to server.\n");
        return -1;
    }
    printf("Connected to server.\n");

    printf("Enter the number of frames: ");
    scanf("%d", &fram);
    printf("Enter the window size: ");
    scanf("%d", &win);

    func(sockdesc, fram, win);
}