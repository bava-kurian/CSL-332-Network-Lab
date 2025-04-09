#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>

struct timeval timeout;

void func(int csock){
    int next = 0, ack = -1;
    char buff[200];
    while(1){
        bzero(buff, 200);
        recv(csock, buff, sizeof(buff), 0);
        int frame = atoi(buff);

        if(frame == -1){
            printf("All frames received.\n");
            break;
        }
        if(next != frame){
            printf("Frame %d received. Discarding frame. ", frame);
            bzero(buff, 200);
            snprintf(buff, sizeof(buff), "%d", frame);
            send(csock, buff, sizeof(buff), 0);
            printf("Acknowlegment sent for %s.\n", buff);
            continue;
        }

        ack = frame;
        // printf("next now: %d\n", next);
        
        srand(time(0));
        int r = rand()%3;
        
        switch(r){
            case 0:
            break;
            
            case 1:
            printf("Frame %d received. ", frame);
            bzero(buff, 200);
            snprintf(buff, sizeof(buff), "%d", frame);
            send(csock, buff, sizeof(buff), 0);
            printf("Ack %s sent.\n", buff);
            next++;
            break;
            
            case 2:
            printf("Frame %d received. ", frame);
            sleep(1);
            bzero(buff, 200);
            snprintf(buff, sizeof(buff), "%d", frame);
            send(csock, buff, sizeof(buff), 0);
            printf("Ack %s sent.\n", buff);
            next++;
            break;
        }

    }
}

int main(void){
    int sockdesc, csock;
    struct sockaddr_in server_addr, client_addr;

    sockdesc = socket(AF_INET, SOCK_STREAM, 0);
    if(sockdesc < 0){
        printf("Couldnt create socket.\n");
        return -1;
    }
    printf("Socket created.\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    timeout.tv_sec = 3;
    timeout.tv_usec = 3;

    if(bind(sockdesc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldnt bind to port.\n");
        return -1;
    }
    printf("Binded to port.\n");

    if(listen(sockdesc, 1) < 0){
        printf("Unable to listen.\n");
        return -1;
    }
    printf("Listening for incoming requests.\n");

    int csize = sizeof(client_addr);
    csock = accept(sockdesc, (struct sockaddr*)&client_addr, &csize);
    if(csock < 0){
        printf("Unable to connect to client.\n");
        return -1;
    }
    printf("Connect to client at IP: %s and port: %i.\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    func(csock);
}