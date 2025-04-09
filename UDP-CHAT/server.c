#include <netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(void)
{
    int sock_desc,client_sock;
    struct sockaddr_in server_addr,cleint_addr;
    char client_msg[1024],server_msg[1024];

    memset(server_msg,'\0',1024);
    memset(client_msg,'\0',1024);

    sock_desc=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sock_desc<0)
    {
        printf("Eror in creating socket!!!\n");
        return -1;
    }
     printf("Socket created successful...\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(bind(sock_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("Error in binding");
        return -1;
    }
    printf("Binding successfull..\n");

    printf("Listeing.....\n");
    sleep(2);
    int c;
    socklen_t server_struct_size=sizeof(server_addr);
    while(1)
    {
        printf("Enter choice\n 1.send msg\n 2.exit\n enter choice:");
        scanf("%d",&c);

        if(c==1)
        {
            memset(client_msg,'\0',1024);
            printf("Enter message to sent:");
            scanf("%s",client_msg);
           if(sendto(sock_desc,client_msg,sizeof(client_msg),0,(struct sockaddr*)&server_addr,server_struct_size)<0)
           {
                printf("couln't send the msg\n");
           }
        }
        else if(c==2)
        {
            printf("Exiting....\n");
            strcpy(client_msg,"Exit");
            sendto(sock_desc,client_msg,sizeof(client_msg),0,(struct sockaddr*)&server_addr,server_struct_size);
            close(sock_desc);
            return 0;
        }
        else
        {
            memset(client_msg,'\0',1024);
            printf("receiving from server...\n");
            if(recvfrom(sock_desc,client_msg,sizeof(client_msg),0,(struct sockaddr*)&server_addr,&server_struct_size)<0)
            {
                printf("Error while receiving server's msg\n");
                return -1;

            }
            printf("server msg:%s\n",client_msg);
            if(strcmp(client_msg,"Exit")==0)
            {
                printf("Exiting from chat\n");
                close(sock_desc);
                return-1;
            }
        }
    }    
}