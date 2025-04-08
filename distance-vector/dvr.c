#include<stdio.h>

int main(void)
{
    int n;
    int cost[100][100];
    struct node
    {
        int distance[100];
        int from[100];
    }r[100];
    printf("Enter number of nodes:");
    scanf("%d",&n);
    printf("Enter the cost metrix:\n");
    for(int i =0;i<n;i++)
    {
         for(int j=0;j<n;j++)
         {
            printf("cost of %d to %d:",i,j);
            scanf("%d",&cost[i][j]);
            r[i].distance[j]=cost[i][j];
            r[i].from[j]=j;
            cost[i][i]=0;
         }
    }
    int count=0;
    do
    {
    for (int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            for(int k=0;k<n;k++)
            {
                if(r[i].distance[j]>cost[i][k]+r[k].distance[j])
                {
                    r[i].distance[j]=r[i].distance[k]+r[k].distance[j];
                    r[i].from[j]=k;
                    count++;
                }
            }
        }
    }

    }while(count!=0);

     for(int i=0;i<n;i++)
        {
            printf("\n\n Routing Table For Router %d\n",i+1);
            for(int j=0;j<n;j++)
            {
                printf("\t\nnode %d via %d Distance %d ",j+1,r[i].from[j]+1,r[i].distance[j]);
            }
        }

}