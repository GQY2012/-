#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define N 30

typedef struct BiTNode{
	char data;
	struct BiTNode*lchild,*rchild;
}BiTNode,*BiTree;

char PreArray[30] = {"\0"};
char InArray[30] = {"\0"};

void PreCreateTree(BiTree &T,int a,int b,int Length){   //a:������������һ���ڵ��±꣬b:������������һ���ڵ��±� 
    if(Length <= 0||strlen(PreArray) != strlen(InArray))
	{  
        T = NULL;
        return ;  
    }  
    else{  
        T = (BiTree) malloc (sizeof(BiTNode));   
        T->data = PreArray[a];  //�����е�һ����ĸ��Ϊ���ڵ��ֵ
        if(!strchr(InArray,PreArray[a])) 
		puts("error");
		int num = strchr(InArray,PreArray[a]) - InArray;    //strchr�����ҵ����ڵ��ֵ���������еĵ�ַ
        int Length_left = num - b;  //�������ڵ��� 
        PreCreateTree(T->lchild,a + 1,b,Length_left);   
        int Length_right = Length - 1 - Length_left;  //�������ڵ��� 
        PreCreateTree(T->rchild,a + Length_left + 1,num + 1,Length_right);  
    } 
}

void PostOrderTraverse_print(BiTree T)
{
	if(T)
	{
		PostOrderTraverse_print(T -> lchild);
		PostOrderTraverse_print(T -> rchild);
		printf("%c",T -> data);
	}
}

main()
{
	BiTree T;
	puts("input PreOrder_string");
	gets(PreArray);
	puts("input InOrder_string");
	gets(InArray);
	PreCreateTree(T,0,0,strlen(InArray));
	puts("PostOrder_string");
	PostOrderTraverse_print(T);	
}

