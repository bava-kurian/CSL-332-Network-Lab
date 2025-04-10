#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>

struct timeval timeout;
char buff[200];

int func(int sock_desc,int frame,int win)
{
    int w1,w2=win,sent=0,yet=0;

    if(setsockopt(sock_desc,SOL_SOCKET,SO_RCVTIMEO,(struct timeval*)&timeout,sizeof(timeout))<0)
    {
        printf("setsockopt(SO_RCVTIMEO) failed.\n");
    }

    while(sent<frame)
    {
        while(yet < win && w1 < frame){
            bzero(buff, 200);
            snprintf(buff, sizeof(buff), "%d", w1);
            send(sock_desc, buff, sizeof(buff), 0);
            sleep(1);
            printf("Frame %d sent.\n", w1);
            w1++;
            yet++;
        }

        recv(sock_desc,buff,sizeof(buff),0);
        int f=atoi(buff);

        if(f!=sent)
        {
            printf("Ack received for frame %d. Frame %d lost.\n",f,sent);
            yet=0;
            w1=sent;
            continue;
        }
        printf("Ack received for frame %d\n",sent);
        yet--;
        sent++;
    }
    snprintf(buff, sizeof(buff), "%d", -1);
    send(sock_desc, buff, sizeof(buff), 0);
    printf("sent %s.\n", buff);
    return 0;
}

int main(void)
{
    int sockdesc,frame,win;
    struct sockaddr_in server_addr;
    
    sockdesc = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(sockdesc < 0){
        printf("Couldnt create socket.\n");
        return -1;
    }
    printf("Created socket.\n");

    timeout.tv_sec =3;
    timeout.tv_usec=0;

    if(connect(sockdesc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect to server.\n");
        return -1;
    }
    printf("Connected to server.\n");

    printf("Enter the number of frames: ");
    scanf("%d", &frame);
    printf("Enter the window size: ");
    scanf("%d", &win);

    func(sockdesc, frame, win);

}