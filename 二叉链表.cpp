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

void PreCreateTree(BiTree &T,int a,int b,int Length){   //a:先序列子树第一个节点下标，b:中序列子树第一个节点下标 
    if(Length <= 0||strlen(PreArray) != strlen(InArray))
	{  
        T = NULL;
        return ;  
    }  
    else{  
        T = (BiTree) malloc (sizeof(BiTNode));   
        T->data = PreArray[a];  //先序列第一个字母即为根节点的值
        if(!strchr(InArray,PreArray[a])) 
		puts("error");
		int num = strchr(InArray,PreArray[a]) - InArray;    //strchr用来找到根节点的值在中序列中的地址
        int Length_left = num - b;  //左子树节点数 
        PreCreateTree(T->lchild,a + 1,b,Length_left);   
        int Length_right = Length - 1 - Length_left;  //右子树节点数 
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

