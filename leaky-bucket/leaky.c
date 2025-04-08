#include<stdio.h>

int main(void)
{
    int in,out,n,bucket=0,bsize;
    printf("enter bucket size:");
    scanf("%d",&bsize);
    printf("enter the number of packets:");
    scanf("%d",&n);
    printf("enter the outgoing rate:");
    scanf("%d",&out);

    while(n!=0)
    {
        printf("Enter the incomming packet:");
        scanf("%d",&in);
        if(in<=(bsize-bucket))
        {
            bucket+=in;
            printf("bucket status: %d / %d \n",bucket,bsize);
        }
        else
        {
            printf("Droppped %d of incomming packet\n",in-(bsize-bucket));
            bucket=bsize;
            printf("bucket status: %d / %d \n",bucket,bsize);
        }
        bucket=bucket-out;
        printf("Outgoing....\n");
        printf("bucket status: %d / %d \n",bucket,bsize);
        n--;
    }
}