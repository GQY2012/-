#include <stdio.h>  
#include <stdlib.h>
#include <malloc.h>
#include <conio.h> 
#include <time.h>
/*ȫ�ֱ���*/
int ArrivalTime=0;      //����ʱ��
	   int business=0;         //�����ҵ��
	   int Duration=0;         //����ҵ������ʱ��
	   int money=0;            //����Ľ��
	   int durtime=0;          //������һ������ʱ���ʱ���� 
	   int num=0;
	   int leavetime=0;
	   int TotalTime=0,CustomerNum=0,Summoney=0,CloseTime=0; 
typedef struct Node{         // �ͻ���¼
       int ArrivalTime;      //����ʱ��
	   int business;         //�����ҵ��
	   int Duration;         //����ҵ������ʱ��
	   int money;            //����Ľ��
	   int num;
}QElemType;
typedef struct QNode         //�ڵ�
{
QElemType elem;
struct QNode *next;
}QNode,*QueuePtr;
typedef struct LinkQueue     //����
{ 
 QueuePtr front;
 QueuePtr rear;
}LinkQueue;
int InitQueue(LinkQueue &Q)            //��ʼ������
{ 
 Q.front=Q.rear=(QueuePtr)malloc(sizeof(QNode));
 if(!Q.front) exit(0);
 Q.front->next=NULL;
 return 1;
}
void EnterQueue(LinkQueue &Q,int money2,int temp)                        //���
{
  QueuePtr p;
  if((p=(QueuePtr)malloc(sizeof(QNode)))== NULL)
  {printf("���ʧ��");exit(0);}
  p->elem.ArrivalTime=ArrivalTime;
  p->elem.Duration=temp;
  p->elem.money=money2;
  p->elem.num=num;
  p->next=NULL;
  Q.rear->next=p;
  Q.rear=p;
  printf("�ɹ����봰��2\n");
  printf("�����Ǯ����%d\n",money2);
  
}

int QLength(LinkQueue &q)              //���г���
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
void Check(LinkQueue &Q,int Summoney)                              //����Ƿ����㴰��2�пͻ���Ҫ��
{  
	int i,t;
	QNode*p=Q.front->next;
	QNode*q;
	printf("��鴰�ڶ�\n");
	printf("��鵽�ͻ�����%d\n",QLength(Q));
	t=QLength(Q);
	for(i=0;i<t;i++)
	{
        printf("��鵽�ͻ���Ǯ����%d\n",p->elem.money);
		ArrivalTime=p->elem.ArrivalTime;
		num=p->elem.num;
		if(p->elem.money<=Summoney) {
			Summoney-=p->elem.money;
            leavetime=leavetime+p->elem.Duration;
            TotalTime += leavetime - ArrivalTime;
			 printf("��%3d�ͻ�����ʱ��%3d����ҵ��2������%5d�����ܽ��%5d�뿪ʱ��%3d\n",num,ArrivalTime,p->elem.money,Summoney,leavetime);
		    Q.front->next=p->next;
		    q=p;
		    free(q);
            p=Q.front->next;
			if(leavetime>CloseTime){
			CustomerNum=CustomerNum-QLength(Q);
             DestoryQueue(Q);
            printf("����Ӫҵ\n");
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
	   printf("������Ӫҵʱ�䣨���ӣ���");
	   scanf("%d",&CloseTime);
	   printf("���г�ʼ��");
	   scanf("%d",&Summoney);
	   printf("����ʱ����������");
	   scanf("%d",&t1);
	   printf("����ҵ����������ʱ�䣺");
	   scanf("%d",&t2);
	   printf("��ʼӪҵ\n");
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
			 if(leavetime>CloseTime) {printf("����Ӫҵ\n");break;}
			 else {
				 TotalTime+=leavetime-ArrivalTime;
                 CustomerNum++;
				 num=CustomerNum;
				 Summoney+=money;
				 printf("��%3d�ͻ�����ʱ��%3d����ҵ��1������%5d�����ܽ��%5d�뿪ʱ��%3d\n",num,ArrivalTime,money,Summoney,leavetime);
				 Check(q,Summoney);
			 }
		   }
		   if(business==2){
			   if(money<=Summoney){
				 if(ArrivalTime>=leavetime)leavetime=ArrivalTime+Duration;
        	     else leavetime=leavetime+Duration;
				 if(leavetime>CloseTime) {CustomerNum=CustomerNum-QLength(q);printf("����Ӫҵ\n");break;}
			     else {
				 TotalTime+=leavetime-ArrivalTime;
                 CustomerNum++;
				 num=CustomerNum;
				 Summoney-=money;
				 printf("��%3d�ͻ�����ʱ��%3d����ҵ��2������%5d�����ܽ��%5d�뿪ʱ��%3d\n",num,ArrivalTime,money,Summoney,leavetime);	 
			   }
			   }
			  else{CustomerNum++;num=CustomerNum;money2=money;temp=Duration;
			  printf("��%3d�ͻ�����ʱ��%3d������%5d ���봰��2�ȴ�\n",num,ArrivalTime,money,Summoney,leavetime);
			  EnterQueue(q,money2,temp); 
				  
				  }
			   }
		   
	   }
	   CustomerNum=CustomerNum-QLength(q);
	   printf("�Ӵ��ͻ�����%d\n", CustomerNum);
	   printf("ƽ��ʱ�䣺%f\n", (float)TotalTime/(float)CustomerNum);

	   
}
