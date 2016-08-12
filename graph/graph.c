#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char citydata[50][50];
int edge=0,citysize=0,distance[50][50]={0};
int level[50];
int visited[50]={0};
int x=0,y=0;
int Xlocation[50];

void open(char filename[]);
void creatlevel(int j, int temp);
void build();
void dfs(int v, int swit);
void Print();
void swap(int j, int i);
void addq(int vertex);
int deleteq();
void bfs(int v);
void dijk(int s, int t);
int IsLoop(int j, int i, int map[citysize][citysize]);
void catstr(char tempstr[],char option[], int swit);
void gotoxy(int x,int y);


typedef struct DATA
{
    char city[20];
    int location;
    int xx;
	int yy;
    struct DATA *next;
}DATA;
DATA **linklist=NULL;

void gotoxy(int x, int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}
void Print()
{
    DATA *dptr;
    int i,j,k;
    printf("\n\n");
    for(i=0;i<citysize;i++)
        printf("%5d ",level[i]);
    printf("\n");
    for(i=0;i<citysize;i++)
    {
        printf("%s ",citydata[i]);
    }
    printf("\n");
    for(j=0;j<citysize;j++)
    {
        for(i=0;i<citysize;i++)
        {
            printf("%5d ",distance[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    for(j=0;j<citysize;j++)
    {
        dptr = linklist[j];
        while(dptr)
        {
            printf("%s ",dptr->city);
            dptr = dptr->next;
        }
        printf("\n");
    }
}

void swap(int j, int i)
{
    char tempstr[20];
    int temp,k;
    strcpy(tempstr,citydata[j]);
    strcpy(citydata[j],citydata[i]);
    strcpy(citydata[i],tempstr);
    for(k=0;k<citysize;k++)
    {
        temp = distance[k][j];
        distance[k][j] = distance[k][i];
        distance[k][i] = temp;
    }
    for(k=0;k<citysize;k++)
    {
        temp = distance[j][k];
        distance[j][k] = distance[i][k];
        distance[i][k] = temp;
    }
    temp = level[j];
    level[j] = level[i];
    level[i] = temp;
}

void open(char filename[])
{
    FILE *fp;

    fp = fopen(filename,"r");

    if(fp==NULL)
    {
        printf("The file %s does not exist!\n",filename);
        return;
    }

    int i,j,k,temp;
    char sample[50],*pch;
    while(fgets(sample,50,fp)!=NULL)
    {
        edge++;
        k=0;
        pch = strtok(sample," ");
        while(pch!=NULL)
        {
            if(k==0)
            {
                for(j=0;j<citysize;j++)
                {
                    if(strcmp((citydata[j]),pch)==0) break;
                }
                if(j>=citysize || citysize==0)
                {
                    strcpy(citydata[j],pch);
                    citysize++;
                }
            }
            else if(k==1)
            {
                for(i=0;i<citysize;i++)
                {
                    if(strcmp((citydata[i]),pch)==0) break;
                }
                if(i>=citysize || citysize==0)
                {
                    strcpy(citydata[i],pch);
                    citysize++;
                }
            }
            else
            {
                temp = atoi(pch);
                distance[j][i] = temp;
            }
            k++;
            pch = strtok(NULL," ");
        }
    }
}

void creatlevel(int j, int temp)
{
    int i;
    if(level[j]>temp) level[j]=temp;
    for(i=0;i<citysize;i++)
    {
        if(distance[j][i]>0)
        {
            creatlevel(i,temp+1);
        }
    }
}

void build()
{
    DATA *dptr,*prev;
    free(linklist);
    linklist = malloc(sizeof(DATA*)*citysize);

    int i,j,k;
    for(j=0;j<citysize;j++)
    {
        dptr =  malloc(sizeof(DATA));
        strcpy(dptr ->city,citydata[j]);
        linklist[j]=dptr;
        for(i=0;i<citysize;i++)
        {
            if(distance[j][i]>0 && (level[j]<level[i] || level[j]-1==level[i]) ) //if two city are connected and (the level not equal or i is j parent)
            {
                prev = dptr;
                dptr =  malloc(sizeof(DATA));
                prev->next = dptr;
                dptr->next = NULL;
                strcpy(dptr ->city,citydata[i]);
            }
        }
    }
}

void dfs(int v, int swit)
{
    DATA *w;
    int i,j;

    visited[v]=1;
	if(swit)    gotoxy(x,y);
    printf("%s\n",citydata[v]);
    for(j=0,w=linklist[v]; w ;w=w->next,j++)
    {
        for(i=0;i<citysize;i++)
        {
            if(strcmp(citydata[i],w->city)==0) break;
        }
        y=level[i]+citysize+3;
		w->location = x;
		x=Xlocation[level[i]]*10;
        if(!visited[i])
        {
			Xlocation[level[i]]++;
        	dfs(i,swit);
		}
    }
}

int queue[50]={0};
int front=0,rear=0;
void addq(int vertex)
{
    queue[rear]=vertex+1;
    rear++;
}
int deleteq()
{
    return queue[front++]-1;
}

void bfs(int v)
{
    DATA *w;
    int i,count=1;
    visited[v]=1;
    printf("%s\n",citydata[v]);
    addq(v);
    while(1)
    {
        if(count==citysize) break;
        v = deleteq();
        for(w=linklist[v]; w ; w=w->next)
        {
            for(i=0;i<citysize;i++)
            {
                if(strcmp(citydata[i],w->city)==0) break;
            }
            if(!visited[i])
            {
                printf("%s\n",citydata[i]);
                count++;
                addq(i);
                visited[i]=1;
            }
        }
    }
}

void dijk(int s, int t)
{
    int i,j,u,v,mini;
    int found[citysize],dis[citysize],parent[citysize];

    for(i=0;i<citysize;i++)
    {
        found[i]=0;
        dis[i]=99999;
    }
    dis[s]=0;
    parent[s]=-1;
    for(i=0;i<citysize;i++)
    {
        for(mini=99999,j=0; j<citysize; j++)/*find the shortest path, and where u is*/
        {
            if(found[j]==0 && dis[j]<mini)
            {
                u=j;
                mini=dis[j];
            }
        }

        if(mini==99999)
        {
            printf("INF");
            return;
        }
        found[u]=1;
        if(u==t)
        {
            printf("mini cost: %d\n",dis[t]);
            break;
        }

        for(v=0;v<citysize;v++)
        {
            if(!found[v] && distance[u][v]>0)
            {
                if(dis[u]+distance[u][v] < dis[v])
                {
                    dis[v]=dis[u] + distance[u][v];
                    parent[v]=u;
                }
            }
        }

    }
    while(t!=s)
    {
        printf("%s <- ",citydata[t]);
        t=parent[t];
    }
    printf("%s\n",citydata[t]);
}

void Kruskal()
{
    int ui,uj,i,j,swit=0,count=0,mini=99999;
    int visit[citysize][citysize], map[citysize][citysize];
    //Print();
    for(j=0;j<citysize;j++)
    {
        for(i=0;i<citysize;i++)
        {
            map[j][i]=0;
            visit[j][i]=0;
        }
        Xlocation[j]=0;
    }
    while(1)
    {
        mini=99999;
        for(j=0;j<citysize;j++)
        {
            for(i=0;i<citysize;i++)
            {
                if(map[j][i]!=0) map[j][i]=1;
            }
        }

        for(j=0;j<citysize;j++)
        {
            for(i=j;i<citysize;i++)
            {
                if(!visit[j][i] && mini>distance[j][i] && distance[j][i])
                {
                    ui=i; uj=j;
                    mini = distance[j][i];
                }
            }
        }
        if(count == edge ) break;
        map[uj][ui]=1;
        map[ui][uj]=1;
        visit[uj][ui]=1;
        swit = IsLoop(uj,ui,map);

        if(!swit)
        {
            printf("%s %s   %d\n",citydata[ui],citydata[uj],mini);
        }
        else
        {
            map[uj][ui]=0;
            map[ui][uj]=0;
        }
        count++;
    }
}

int IsLoop(int j, int i, int map[citysize][citysize])
{
    int ii, swit=0;
    for(ii=0 ; ii<citysize; ii++)
    {
        if(ii==i) continue;  //if ii == parent
        if(map[j][ii]>0)
        {
            map[j][ii]=-1;
            swit = IsLoop(ii, j, map);
            if(swit) return 1;
        }
        else if(map[j][ii]==-1) return 1;
    }
    return 0;
}

void catstr(char tempstr[],char option[], int swit)
{
    int i,j=0;
    for(i=0;i<strlen(option);i++)
    {
        if(swit==1)
        {
            if(option[i]!=' ')
            {
                tempstr[j]=option[i];
                tempstr[j+1]='\0';
                j++;
            }
        }
        if(option[i]==' ')swit++;
    }
}

int main(int argc , char **argv)
{
	DATA *w;
    int i,j,k,temp,mini;
    char filename[20],tempstr[50];
    system("clear");
    strcpy(filename,argv[1]);
    open(filename);
    for(i=0;i<citysize;i++) level[i]=99999;
    creatlevel(0,1);
    gotoxy(0,20);
	for(i=0;i<citysize;i++)printf("%s %d\n",citydata[i],level[i]);

    //sort distance by level
    for(j=0;j<citysize;j++)
    {
        for(i=0;i<citysize;i++)
        {
            if(level[j] < level[i]) swap(j,i);
            else if(level[j] == level[i])
            {
                if(strcmp(citydata[j],citydata[i])<0 )swap(j,i);
            }
        }
    }

    for(j=0;j<citysize;j++)
    {
        for(i=0;i<citysize;i++)
        {
            if(distance[j][i]>0)
                distance[i][j] = distance[j][i];
        }
    }

    build();
	for(j=0;j<citysize;j++)
	{
		for(i=0;i<citysize;i++)
		{
			if(i==j) continue;
			w = linklist[i];
			while(w)
			{
				if(strcmp(linklist[j]->city, w->city)==0)
				{
					w->location = linklist[j]->location;
				}
				w=w->next;
			}
		}
	}

	char option[20];
    while(1)
    {
    	for(i=0;i<citysize;i++)
	    {
    	 	Xlocation[i]=0;
        	visited[i]=0;
	    }
        x=0; y=level[0]+citysize+3;
        dfs(0,1);
    
    	for(i=0;i<citysize;i++)
        {
        	Xlocation[i]=0;
            visited[i]=0;
        }
        gotoxy(0,0);
        printf(">");
        gets(option);
        system("clear");
        gotoxy(0,2);
        switch(option[0])
        {
        	case 'q':
        		printf("Bye~\n");
        		return 0;
            case 'D':
                catstr(tempstr,option,0);
                for(i=0;i<citysize;i++)
                    if(strcmp(tempstr,citydata[i])==0) break;
                if(i>citysize)
   	            {
       	            printf("City %s does not exist!\n\n",tempstr);
           	        break;
                }
                y=level[i]+citysize+3;
        		x=0;
        		linklist[i]->location=x;
                dfs(i,0);
				
            break;

            case 'B':
                catstr(tempstr,option,0);
                for(i=0;i<citysize;i++)
                    if(strcmp(tempstr,citydata[i])==0) break;
                if(i>=citysize)
                {
                    printf("City %s does not exist!\n\n",tempstr);
                    break;
                }
                bfs(i);
            break;

            case 'S':
                catstr(tempstr,option,0);
                for(i=0;i<citysize;i++)
                    if(strcmp(tempstr,citydata[i])==0) break;

                catstr(tempstr,option,-1);
                for(j=0;j<citysize;j++)
                    if(strcmp(tempstr,citydata[j])==0) break;
                if(i>=citysize || j>=citysize)
                {
                    printf("City does not exist!\n\n");
                    break;
                }
                dijk(i,j);
            break;

            case 'M':
            	printf("Edge\t\tWeight\n");
                Kruskal();
            break;
        }
        printf("\n\n");
    }

    return 0;
}



