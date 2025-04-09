#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>
#include<sys/time.h>

struct timeval timeout;

char buff[200];

void func(int csock){
    srand(time(0));
    int next=0, ack=-1;

    while(1){
        bzero(buff, sizeof(buff));
        recv(csock, buff, sizeof(buff), 0);
        int frame = atoi(buff);

        if(frame == -1){
            printf("All frames received.\n\n");
            break;
        }
        // if(frame < next){
        //     continue;
        // }
        // if(frame != next){
        //     printf("Frame %d received.\n", frame);
        //     bzero(buff, sizeof(buff));
        //     snprintf(buff, sizeof(buff), "%d", frame);
        //     send(csock, buff, sizeof(buff), 0);
        //     printf("Ack sent for %d. Discarding frames.\n", frame);
        //     continue;
        // }
        
        ack = frame;
    
        int r = rand() % 3;
    
        switch(r){
            case 0:
            printf("Frame %d not received.\n", frame);
            bzero(buff, sizeof(buff));
            snprintf(buff, sizeof(buff), "%d", frame);
            // send(csock, buff, sizeof(buff), 0);
            // printf("Ack sent for %d. Discarding frames.\n", frame);
            break;
    
            case 1:
            printf("Frame %d received. ", frame);
            bzero(buff, sizeof(buff));
            snprintf(buff, sizeof(buff), "%d", ack);
            send(csock, buff, sizeof(buff), 0);
            printf("Ack %d sent.\n", ack);
            next++;
            break;
            
            case 2:
            printf("Frame %d received. ", frame);
            sleep(1);
            bzero(buff, sizeof(buff));
            snprintf(buff, sizeof(buff), "%d", ack);
            send(csock, buff, sizeof(buff), 0);
            printf("Ack %d sent.\n", ack);
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
        printf("couldnt create socket.\n");
        return -1;
    }
    printf("Socket created.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if(bind(sockdesc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldnt binf to port.\n");
        return -1;
    }
    printf("Binded to port.\n");

    if(listen(sockdesc, 1) < 0){
        printf("Unable to listen.\n");
        return -1;
    }
    printf("Listening...\n");
    
    int csize = sizeof(client_addr);
    csock = accept(sockdesc, (struct sockaddr*)&client_addr, &csize);
    if(csock < 0){
        printf("Couldnt connect to client.\n");
        return -1;
    }
    printf("Connected to client.\n");

    func(csock);
    close(sockdesc);
    close(csock);
}