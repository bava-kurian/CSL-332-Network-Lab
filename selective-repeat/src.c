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

void func(int sockdesc, int fram, int win){
    int ackarr[fram];
    for(int i=0; i<fram; i++){
        ackarr[i] = -1;
    }

    int start=0, stop=win, next=0, sent=0, items=0, ptr=0;
    if(setsockopt(sockdesc, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&timeout, sizeof(timeout)) < 0){
        printf("Error setting timeout.\n");
    }
    while(sent < fram){

        while(items<win && start<fram){
            bzero(buff, sizeof(buff));
            snprintf(buff, sizeof(buff), "%d", start);
            send(sockdesc, buff, strlen(buff), 0);
            sleep(1);
            printf("Frame %d sent.\n", start);
            ackarr[ptr] = start;
            ptr++;
            items++;
            start++;
        }

        int n = recv(sockdesc, buff, sizeof(buff), 0);
        if(n <= 0){
            // timeout or error
            printf("Timeout occurred. Resending pending frames.\n");
            for(int i=0; i<fram; i++){
                if(ackarr[i] != -1){  // resend only valid frames
                    bzero(buff, sizeof(buff));
                    snprintf(buff, sizeof(buff), "%d", ackarr[i]);
                    send(sockdesc, buff, strlen(buff), 0);
                    sleep(1);
                    printf("Frame %d resent.\n", ackarr[i]);
                }
            }
            continue;
        }

        int frame = atoi(buff);
        
        // if(frame != sent){
        //     printf("Ack for %d received. Frame %d lost. Resending frames.\n", frame, sent);
        //     items = 0;
        //     start = sent;
        //     continue;
        // }
        // printf("Ack received for frame %d.\n", frame);
        // items--;
        // sent++;
        printf("Ack received for frame %d.\n", frame);
        for(int i=0; i<fram; i++){
            if(ackarr[i] == frame){
                ackarr[i] = -1;
                if(start < fram){
                    ackarr[ptr] = start;
                    ptr++;

                    bzero(buff, sizeof(buff));
                    snprintf(buff, sizeof(buff), "%d", start);
                    send(sockdesc, buff, sizeof(buff), 0);
                    sleep(1);
                    printf("Frame %d sent.\n", start);
                    start++;
                }
                sent++;
                break;
            }
        }

        if(sent == fram){
            break;
        }
    }

    bzero(buff, sizeof(buff));
    snprintf(buff, sizeof(buff), "%d", -1);
    send(sockdesc, buff, sizeof(buff), 0);
    printf("Done...\n");
}

int main(void){
    int sockdesc;
    struct sockaddr_in server_addr;
    int fram, win;
    
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

    if(connect(sockdesc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldnt connect to server.\n");
        return -1;
    }
    printf("Connected to server.\n");

    printf("Enter the number of frames: ");
    scanf("%d", &fram);
    printf("Enter the window size: ");
    scanf("%d", &win);

    func(sockdesc, fram, win);
    close(sockdesc);
}