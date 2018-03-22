#include <stdio.h>  
#include <stdlib.h>
#include <malloc.h>
#include <conio.h> 
#include <time.h>
/*全局变量*/
int ArrivalTime=0;      //到达时刻
	   int business=0;         //办理的业务
	   int Duration=0;         //办理业务所需时间
	   int money=0;            //办理的金额
	   int durtime=0;          //距离上一个到达时间的时间间隔 
	   int num=0;
	   int leavetime=0;
	   int TotalTime=0,CustomerNum=0,Summoney=0,CloseTime=0; 
typedef struct Node{         // 客户记录
       int ArrivalTime;      //到达时刻
	   int business;         //办理的业务
	   int Duration;         //办理业务所需时间
	   int money;            //办理的金额
	   int num;
}QElemType;
typedef struct QNode         //节点
{
QElemType elem;
struct QNode *next;
}QNode,*QueuePtr;
typedef struct LinkQueue     //队列
{ 
 QueuePtr front;
 QueuePtr rear;
}LinkQueue;
int InitQueue(LinkQueue &Q)            //初始化队列
{ 
 Q.front=Q.rear=(QueuePtr)malloc(sizeof(QNode));
 if(!Q.front) exit(0);
 Q.front->next=NULL;
 return 1;
}
void EnterQueue(LinkQueue &Q,int money2,int temp)                        //入队
{
  QueuePtr p;
  if((p=(QueuePtr)malloc(sizeof(QNode)))== NULL)
  {printf("入队失败");exit(0);}
  p->elem.ArrivalTime=ArrivalTime;
  p->elem.Duration=temp;
  p->elem.money=money2;
  p->elem.num=num;
  p->next=NULL;
  Q.rear->next=p;
  Q.rear=p;
  printf("成功进入窗口2\n");
  printf("进入的钱数：%d\n",money2);
  
}

int QLength(LinkQueue &q)              //队列长度
{ 
QNode*qtemp;
int i=0;
qtemp = q.front->next;
while(qtemp)
{
qtemp = qtemp->next;
i++;
}
return i;
}
void DestoryQueue(LinkQueue &q)
{
QNode*p;
while(q.front->next)
{
p = q.front->next;
q.front->next = p->next;
free(p);
}
q.front->next=NULL;
q.rear = q.front;
}
void Check(LinkQueue &Q,int Summoney)                              //检测是否满足窗口2中客户的要求
{  
	int i,t;
	QNode*p=Q.front->next;
	QNode*q;
	printf("检查窗口二\n");
	printf("检查到客户数：%d\n",QLength(Q));
	t=QLength(Q);
	for(i=0;i<t;i++)
	{
        printf("检查到客户的钱数：%d\n",p->elem.money);
		ArrivalTime=p->elem.ArrivalTime;
		num=p->elem.num;
		if(p->elem.money<=Summoney) {
			Summoney-=p->elem.money;
            leavetime=leavetime+p->elem.Duration;
            TotalTime += leavetime - ArrivalTime;
			 printf("第%3d客户到达时间%3d办理业务2办理金额%5d银行总金额%5d离开时间%3d\n",num,ArrivalTime,p->elem.money,Summoney,leavetime);
		    Q.front->next=p->next;
		    q=p;
		    free(q);
            p=Q.front->next;
			if(leavetime>CloseTime){
			CustomerNum=CustomerNum-QLength(Q);
             DestoryQueue(Q);
            printf("结束营业\n");
			break;			 
			}
		} 
		else if(p->next!=NULL){
		Q.front->next=p->next;
        Q.rear->next=p;
        Q.rear=p;
		Q.rear->next=NULL;
		p=Q.front->next;
		}
		if(QLength(Q)==0)Q.rear=Q.front;
	}
	
}
int main(){
	int money2;
	int temp;
	int t1,t2;
       LinkQueue q;
	   InitQueue(q);
	   printf("请输入营业时间（分钟）：");
	   scanf("%d",&CloseTime);
	   printf("银行初始金额：");
	   scanf("%d",&Summoney);
	   printf("到达时间的最大间隔：");
	   scanf("%d",&t1);
	   printf("办理业务所需的最大时间：");
	   scanf("%d",&t2);
	   printf("开始营业\n");
	   srand((unsigned) time(NULL));
	   while(1){
		   
		   int durtime = rand()%t1+1;
           int Duration = rand()%t2+1; 
           int business= rand()%2+1; 
           int money=rand()%10000+1;
		   ArrivalTime+=durtime;
		   if(business==1){
		     if(ArrivalTime>=leavetime)leavetime=ArrivalTime+Duration;
        	 else leavetime=leavetime+Duration;
			 if(leavetime>CloseTime) {printf("结束营业\n");break;}
			 else {
				 TotalTime+=leavetime-ArrivalTime;
                 CustomerNum++;
				 num=CustomerNum;
				 Summoney+=money;
				 printf("第%3d客户到达时间%3d办理业务1办理金额%5d银行总金额%5d离开时间%3d\n",num,ArrivalTime,money,Summoney,leavetime);
				 Check(q,Summoney);
			 }
		   }
		   if(business==2){
			   if(money<=Summoney){
				 if(ArrivalTime>=leavetime)leavetime=ArrivalTime+Duration;
        	     else leavetime=leavetime+Duration;
				 if(leavetime>CloseTime) {CustomerNum=CustomerNum-QLength(q);printf("结束营业\n");break;}
			     else {
				 TotalTime+=leavetime-ArrivalTime;
                 CustomerNum++;
				 num=CustomerNum;
				 Summoney-=money;
				 printf("第%3d客户到达时间%3d办理业务2办理金额%5d银行总金额%5d离开时间%3d\n",num,ArrivalTime,money,Summoney,leavetime);	 
			   }
			   }
			  else{CustomerNum++;num=CustomerNum;money2=money;temp=Duration;
			  printf("第%3d客户到达时间%3d办理金额%5d 进入窗口2等待\n",num,ArrivalTime,money,Summoney,leavetime);
			  EnterQueue(q,money2,temp); 
				  
				  }
			   }
		   
	   }
	   CustomerNum=CustomerNum-QLength(q);
	   printf("接待客户数：%d\n", CustomerNum);
	   printf("平均时间：%f\n", (float)TotalTime/(float)CustomerNum);

	   
}
