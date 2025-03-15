#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

int main(void)
{
	int socket_desc,client_sock,client_size;
	struct sockaddr_in server_addr,client_addr;
	char buffer[1024];
	int m=1,p,k=5;
	
	socket_desc=socket(AF_INET, SOCK_STREAM,0);
	if(socket_desc<0)
	{
		printf("Error while creating socket\n");
		return -1;
	}
	printf("Scoket Created \n");
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(2000);
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		printf("Couldn't bind to port\n");
		return -1;
	}
	printf("Done with binding\n");
	if(listen(socket_desc,1)<0)
	{
		printf("Error while listening\n");
		return -1;
	}
	printf("\n Listening for incoming connections.....\n");
	client_size=sizeof(client_addr);
	client_sock=accept(socket_desc,(struct sockaddr*)&client_addr,&client_size);
	if(client_sock<0)
	{
		printf("Can't accept\n");
		return -1;
	}
	printf("Client connected at IP: %s and port: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	
	while(k!=0)
	{
		int y=recv(client_sock,buffer,1024,0);
		if(y==-1)
		{
			printf("Error in receiving");
			exit(1);
		}
		if(strcmp(buffer,"frame")==0)
		{
			printf("Received frame %d successfully\n",m);
		}
		else
		{
			printf("Frame %d not received\n",m);
		}
		if(m%2==0)
		{
			strcpy(buffer,"ack");
		}
		else
		{
			strcpy(buffer,"kca");
			printf("Ack lost\n");
			for(p=1;p<=3;p++)
			{
				printf("Waiting for %d seconds\n",p);
			}
			printf("Retransmitting ack...\n");
			strcpy(buffer,"ack");
			sleep(3);
		}
		printf("sending ack %d\n",m);
		int z=send(client_sock,buffer,19,0);
		if(z==-1)
		{
			printf("Error in sending\n");
			exit(1);
		}
		k--;
		m++;
	}   
	close(client_sock);
	close(socket_desc);
	
	return 0;
	
	
}
