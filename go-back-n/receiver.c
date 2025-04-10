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
char buff[200];

int func(int socket_desc)
{
    int next=0,ack=-1;

    while(1)
    {
        bzero(buff,200);
        recv(socket_desc,buff,200,0);
        int frame=atoi(buff);

        if(frame==-1)
        {
            printf("All frames received.\n");
            break;
        }
        if(next!=frame)
        {
            printf("Frame %d received. Discarding frame.\n",frame);
            bzero(buff,200);
            snprintf(buff,sizeof(buff),"%d",frame);
            send(socket_desc,buff,sizeof(buff),0);
            printf("Acknowlegment sent for %s.\n",buff);
            continue;
        }

        ack=frame;
        srand(time(0));
        int r=rand()%3;

        switch(r)
        {
            case 0:
            break;
            case 1:
            sleep(1);
            printf("Frame %d received.\n",frame);
            bzero(buff,200);   
            snprintf(buff,200,"%d",frame);
           if( send(socket_desc,buff,sizeof(buff),0)<0)
           {
            printf("errot in send ack\n");
           }
            printf("Ack %s sent.\n",buff);
            next++;
            break;
            case 2: 
            printf("Frame %d received.\n",frame);
            bzero(buff,200);
            snprintf(buff,200,"%d",frame);
            if( send(socket_desc,buff,sizeof(buff),0)<0)
           {
            printf("errot in send ack\n");
           }
            printf("Ack %s sent.\n",buff);
            next++;
            break;
        }

    }

    return 0;
}

int main(void)
{
    int socket_desc,client_sock;
    int client_size;
    struct sockaddr_in server_addr,client_addr;
    socket_desc=socket(AF_INET,SOCK_STREAM,0);
    if(socket_desc<0)
    {
        printf("Couldnt create socket.\n");
        return -1;
    }
    printf("Created socket.\n");

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(2000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("Couldnt bind to port.\n");
        return -1;
    }
    printf("Binded to port.\n");

    if(listen(socket_desc,1)<0)
    {
        printf("Unable to listen.\n");
        return -1;
    }
    printf("Listening for incoming requests.\n");

    client_size=sizeof(client_addr);
    client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);
    if(client_sock<0)
    {
        printf("Unable to connect to client.\n");
        return -1;
    }
    printf("Connect to client at IP: %s and port: %i.\n",inet_ntoa(client_addr.sin_addr),htons(client_addr.sin_port));

    func(client_sock);
    return 0;
}