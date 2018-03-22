#include "stdio.h"
#include "malloc.h"
#include "stdlib.h"
#include "string.h"

typedef struct customer{	   //�ͻ���Ϣ
	char name[20];			   //�ͻ�����
	int num;				      //��Ʊ��
	int level;				  //��λ�ȼ�
	customer *next;
}customer,*Lcustomer;

typedef struct airline{		//������Ϣ
	char end_addr[20];		//�����յ�
	char line_num[5];		   //�����
	char plant_num[8];		//�ɻ���
	int day;				       //��������
	int total;				    //��Ա
	int left;				    //ʣ��Ʊ��
	customer *booked;		    //�Ѷ��ͻ���Ϣ
	customer *wait;			//�ŶӵȺ�Ʊ��Ϣ
	airline *next;
}airline,*Lairline;

airline *L;					 //��������Ϣ����Ϊȫ�ֱ��������ٲ�������
Lairline search_line()		//������Ų��Һ���
{
	char a[5];
	airline *p;
	p=L->next;
	printf("�����뺽���");
	scanf("%s",a);
	for(;p;p=p->next)
		if(!strcmp(a,p->line_num)) break;
	return p;
}

Lairline search_plant()		//���ɻ��Ų��Һ���
{
	char a[8];
	airline *p;
	p=L->next;
	printf("������ɻ���");
	scanf("%s",a);
	for(;p;p=p->next)
		if(!strcmp(a,p->plant_num)) break;
	return p;
}

Lairline search_day()		//���������ڲ��Һ���
{
	int a;
	airline *p;
	p=L->next;
	printf("�������������");
	scanf("%d",&a);
	for(;p;p=p->next)
		if(a==p->day) break;
	return p;
}

Lairline search_addr()		//�������յ���Һ���
{
	char a[20];
	airline *p;
	p=L->next;
	printf("�����뺽���յ�");
	scanf("%s",a);
	for(;p;p=p->next)
		if(!strcmp(a,p->end_addr)) break;
	return p;
}

void guide()			//���˵�����
{
	printf("**********************��ӭ���뺽����Ʊϵͳ**********************\n");
	printf("\t\t��ʾ���к���==>  1\n");
	printf("\t\t��ѯ������Ϣ==>  2\n");
	printf("\t\t��Ʊҵ��    ==>  3\n");
	printf("\t\t��Ʊҵ��    ==>  4\n");
	printf("\t\t�˳�ϵͳ    ==>  0\n");
	printf("**************************************************************\n");
	printf("\t\t�����Ļ��Ϣ==>  9\n\n");
	printf("�밴����ָ������:\t");
}

void print_customer(airline *p)		//��ʾ����ͻ���Ϣ
{
	customer *c;
	if(p->booked)
	{
		c=p->booked;
		printf("�Ѷ�Ʊ�ͻ���Ϣ\nƱ��\t/\t�ͻ�����\n");
		for(;c;c=c->next)
			printf("%5d\t/\t%s\n",c->num,c->name);
		printf("\n\n");
	}
	else
		printf("��ʱû�пͻ���Ʊ\n\n");
	if(p->wait)
	{
		c=p->wait;
		printf("�ŶӶ�Ʊ�ͻ���Ϣ\nƱ��\t/\t�ͻ�����\n");
		for(;c;c=c->next)
			printf("%5d\t/\t%s\n",c->num,c->name);
		printf("\n\n");
	}
	else
		printf("��ʱû�пͻ��ŶӶ�Ʊ\n\n");
}

void book()				//��Ʊϵͳ
{
	int i;
	int flag=0;
	airline *p;
	customer *cst,*c;
	printf("�������ѯ����ķ�ʽ��\n");	
	printf("�����--->1\n");
	printf("�յ�վ--->2\n");
	printf("��������->3\n");
	for(;!flag;)
	{
		scanf("%d",&i);
		switch(i)
		{
		case 1: p=search_line();flag=1;break;
		case 2: p=search_addr();flag=1;break;
		case 3: p=search_day();flag=1;break;
		default:printf("��������,����������\n");break;
		}
	}
	if(p)
	{

		printf("�յ�վ                  �����   �ɻ���   ��������   ��Ʊ��\n");
		printf("%-20s%10s%10s%8d%8d\n",p->end_addr,p->line_num,p->plant_num,p->day,p->left);
		cst=(customer *)malloc(sizeof(customer));
		printf("�����붩Ʊ������");
		scanf("%d",&cst->num);
		printf("�������λ�ȼ���1/2/3��");
		scanf("%d",&cst->level);
		printf("��������������");
		scanf("%s",cst->name);
		if(cst->num<=p->left)
		{
			cst->next=p->booked;
			p->booked=cst;
			p->left=p->left-cst->num;
			printf("��Ʊ�ɹ�����λ��Ϊ%d~%d\n",p->total-p->left-cst->num+1,p->total-p->left);
		}
		else
		{
			printf("��Ʊ���㣬�Ƿ��ŶӵȺ�\n�ǣ�1��\n��0��\n");
			scanf("%d",&flag);
			if(flag)
			{
				if(!p->wait)
				{
					cst->next=p->wait;
					p->wait=cst;
				}
				else
				{
					c=p->wait;
					for(;c->next;c=c->next);
					cst->next=c->next;
					c->next=cst;
				}
			}
		}
	}
	else 
		printf("û�иú���\n");
	guide();
}

void waited(airline *p)			//�ŶӶ�Ʊϵͳ
{
	int flag;
	customer *q,*q0,*h;
	if(!p->wait) return;
	q=(customer *)malloc(sizeof(customer));
	q->level=0;
	q->num=123;
	strcpy(q->name,"00000");
	q->next=p->wait;
	h=q;
	for(;p->left>0&&q->next;)
	{
		if(q->next->num<p->left)
		{
			printf("%s���������ŶӶ�%d��Ʊ������Ʊ���Ƿ�Ʊ\n�ǣ�1��\n��0��\n",q->next->name,q->next->num);
			scanf("%d",&flag);
			if(flag)
			{
				q0=q->next;
				q->next=q->next->next;
				q0->next=p->booked;
				p->booked=q0;
				p->left=p->left-q0->num;
				printf("�ŶӶ�Ʊ�ɹ�\n\n");
			}
			else
				q=q->next;
		}
		else
			q=q->next;
	}
	p->wait=h->next;
}
void refund()			//��Ʊϵͳ
{
	int flag;
	char a[20];
	airline *p;
	customer *c,*c1;
	p=search_line();
	c=p->booked;
	if(!p)
	{	
		printf("û�иú���\n\n");
		guide();
		return;
	}
	printf("��������������");
	scanf("%s",a);
	if(!p->booked->name)
	{
		printf("�Բ���û���ҵ�������Ϣ\n\n");
		guide();
		return ;
	}
	if(!strcmp(a,p->booked->name))
	{
		printf("��Ʊ��Ϣ\n����/�յ�վ/Ʊ��\n%s/%s/%d\n",p->booked->name,p->end_addr,p->booked->num);
		printf("ȷ��Ҫ��Ʊ������\n�ǣ�1��\n��0)\n");
		scanf("%d",&flag);
		if(flag)
		{
			p->booked=p->booked->next;
			p->left=p->left+c->num;
			free(c);
			printf("��Ʊ�ɹ�\n\n");
			waited(p);
			guide();
		}
	}
	else
	{
		for(;c->next;c=c->next)
			if(!strcmp(a,c->next->name))
				break;
		if(!c->next)
		{
			printf("�Բ���û���ҵ�������Ϣ\n\n");
			guide();
			return ;
		}
		else
		{
			printf("��Ʊ��Ϣ\n����/�յ�վ/Ʊ��\n%s/%s/%d\n",c->next->name,p->end_addr,c->next->num);
			printf("ȷ��Ҫ��Ʊ������\n�ǣ�1��\n��0)\n");
			scanf("%d",&flag);
			if(flag)
			{
				c1=c->next;
				c->next=c->next->next;
				p->left=p->left+c1->num;
				free(c1);
				printf("��Ʊ�ɹ�\n\n");
				waited(p);
				guide();
			}
		}
	}

}

void search()			//�����ѯϵͳ
{
	int i;
	int flag=0;
	airline *p;
	printf("�������ѯ����ķ�ʽ��\n");
	printf("�����---->1\n");
	printf("�ɻ���---->2\n");
	printf("��������-->3\n");
	printf("�����յ�-->4\n");
	scanf("%d",&i);
	for(;!flag;)
		switch(i)
		{
		case 1: p=search_line();flag=1;break;
		case 2: p=search_plant();flag=1;break;
		case 3: p=search_day();flag=1;break;
		case 4: p=search_addr();flag-=1;break;
		default:printf("��������,����������\n");break;
		}
	if(p)
	{
		printf("�յ�վ                 �����   �ɻ���   ��������   ��Ʊ��\n");
		printf("%-20s%10s%10s%8d%8d\n",p->end_addr,p->line_num,p->plant_num,p->day,p->left);
		printf("�Ƿ�Ʊ��ȷ���밴1��");
		scanf("%d",&i);
		if(i==1)
				book();
	}
	else 
		printf("û�иú���\n");
}

void InitLine()				//��ʼ��������Ϣ
{
	airline *p,*q;
	L=(airline *)malloc(sizeof(airline));
	L->booked=NULL;
	L->wait=NULL;
	L->next=NULL;
	strcpy(L->end_addr,"00000000");
	strcpy(L->line_num,"000");
	strcpy(L->plant_num,"0000000");
	L->day=L->left=L->total=0;
	q=L;
    p=(airline *)malloc(sizeof(airline));
	p->booked=NULL;
	p->wait=NULL;
	strcpy(p->end_addr,"���");
	strcpy(p->line_num,"004");
	strcpy(p->plant_num," 3667894");
	p->day=4;
	p->left=120;
	p->total=120;
	p->next=q->next;
	q->next=p;

	p=(airline *)malloc(sizeof(airline));
	p->booked=NULL;
	p->wait=NULL;
	strcpy(p->end_addr,"����");
	strcpy(p->line_num,"003");
	strcpy(p->plant_num,"4558934");
	p->day=3;
	p->left=120;
	p->total=120;
	p->next=q->next;
	q->next=p;

	p=(airline *)malloc(sizeof(airline));
	p->booked=NULL;
	p->wait=NULL;
	strcpy(p->end_addr,"�Ϻ�");
	strcpy(p->line_num,"002");
	strcpy(p->plant_num,"2682154");
	p->day=2;
	p->left=120;
	p->total=120;
	p->next=q->next;
	q->next=p;

   p=(airline *)malloc(sizeof(airline));
	p->booked=NULL;
	p->wait=NULL;
	strcpy(p->end_addr,"����");
	strcpy(p->line_num,"001");
	strcpy(p->plant_num," 2586934");
	p->day=1;
	p->left=120;
	p->total=120;
	p->next=q->next;
	q->next=p;
} 

void printline()			//��ʾ���к�����Ϣ
{
	int flag;
	airline *p;
	p=L->next;
	printf("�յ�վ                  �����   �ɻ���   ��������   ��Ʊ��\n");
	for(;p;p=p->next)
	{
		printf("%-20s%10s%10s%8d%8d\n",p->end_addr,p->line_num,p->plant_num,p->day,p->left);

	}
	printf("��1���붩Ʊϵͳ����0�������˵�\n");
	scanf("%d",&flag);
	if(flag)
		book();
	else guide();

}

main()				//������
{
	int flag=1;
	guide();
	InitLine();	
	do
	{		
		switch(getchar())
		{
		case '1':printline();break;
		case '2':search();break;
		case '3':book();break;
		case '4':refund();break;
		case '9':system("cls");guide();break;
		case '0':flag=0;break;
		}
	}while(flag);
}

