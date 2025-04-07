#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>


int main(void)
{
    FILE *fp;
    int n,s;
    int socket_desc;
    struct sockaddr_in server_addr;
    char name[100],buffer[100],rec[100];

    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc==-1)
    {
        printf("Could not create socket");
        return -1;
    }
    printf("Socket created successfully\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(connect(socket_desc,(struct sockaddr* )&server_addr,sizeof(server_addr))<0)
    {
        printf("error in connecting\n");
        return -1;
    }
    printf("Connected\n");
    
    
    printf("Enter file name:");
    scanf("%s",name);
    name[strlen(name)]='\0';
    fp=fopen(name,"w");

    send(socket_desc,name,strlen(name),0);
    memset(rec,'\0',sizeof(rec));
    while(1)
    {
        if(recv(socket_desc,rec,sizeof(rec),0)<0)
        {
            printf("error in receiving\n");
            break;
        }
        if(strcmp(rec,"error")==0)
        {
            printf("file not found\n");
            break;
            
        }
        else if(strcmp(rec,"completed")==0)
        {
            printf("file transfer completed\n");
            break;
        }
        else
        {
            printf("file conenet:%s",rec);
            fputs(rec,fp);
            break;
        }
    }

    close(socket_desc);
    return 0;
}