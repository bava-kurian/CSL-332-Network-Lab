#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
struct timeval timeout;

int func(int sock_desc)
{
    char buff[1024];
    int f,c,next=0;

    while(1)
    {
        sleep(0);
        bzero(buff,1024);
        recv(sock_desc,buff,1024,0);

        if(strcmp(buff,))
    }
}

int main(void)
{
    int sock_desc,ws,client_sock;
    struct sockaddr_in server_addr,client_addr;
    int client_length;

    sock_desc=socket(AF_INET,SOCK_STREAM,0);

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(bind(sock_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))>0)
    {
        printf("error in binding\n");
        return -1;
    }
    printf("binding successfull..\n");

    if(listen(sock_desc,0)<0)
    {
        printf("error in listening\n");
        return -1;
    }
    printf("Listening...\n");
    client_length=sizeof(client_addr);
    client_sock=accept(sock_desc, (struct sockaddr*)&client_addr,&client_length);
    
    if(client_sock<0)
    {
        printf("Cant accept\n");
        return -1;
    }
    printf("Client Accepted..\n");

    printf("Enter window Size:");
    scanf("%d",&ws);

    func(sock_desc);
}

