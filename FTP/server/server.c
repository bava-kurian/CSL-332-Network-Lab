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
    int socket_desc,client_size,client_sock;
    struct sockaddr_in server_addr,client_addr;
    char name[100],buffer[100],read[100];

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

    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("error in binding\n");
        return -1;
    }
    printf("Binding successful\n");

    if(listen(socket_desc,1)<0)
    {
        printf("error in listening\n");
        return -1;
    }
    printf("Listening\n");

    client_size=sizeof(client_size);
    client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);

    //printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Connection accepted\n");
    n=recv(client_sock,name,sizeof(name),0);
    name[n]='\0';

    printf("message received\n");
    printf("File name:%s\n",name);
    
    fp=fopen(name,"r");
    if(fp==NULL)
    {
        printf("file not found\n");
        send(client_sock,"error",5,0);
        return -1;
    }
    else
    {
        while(fgets(read,100,fp))
        {
            printf("%s",read);
            if(send(client_sock,read,strlen(read),0)<0)
            {
                printf("error in sending\n");
                sleep(1);
            }
        }

        if(!fgets(read,100,fp))
        {
            printf("Done");
            send(client_sock,"completed",strlen("completed"),0);
        }
    }
    
    
    close(socket_desc);
    return 0;
}