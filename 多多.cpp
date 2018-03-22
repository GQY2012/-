#include<iostream.h>
#include<stdlib.h>
#include<math.h>
#include<stdio.h>
 
typedef struct Term{
 
	float coef;
	int exp;
	struct Term * link;
}*Polynomial;
void Input(Polynomial&PL){
 
	 Term*nTerm,*p; 
	 int n;
	 p=PL;
	 cerr<<"请输入多项式的项数:";
	 cin>>n;
	 cerr<<"请输入多项式:\n";
	while(1) {
	 nTerm=new Term;
	 if(!nTerm){cerr<<"存储分配失败！\n";exit(1);}
	 nTerm->link=NULL;
	 cerr<<"输入系数\n";
	  cin>>nTerm->coef;
	 cerr<<"输入指数\n";
     cin>>nTerm->exp;
	 p->link=nTerm;
	 p=nTerm;
	 n--;
	 if(n==0)break;
	 if(n)continue;
	 }
	 
};
void Output(Polynomial&PL){

	Term*p=PL->link;
	cout<<"The polynomial is:"<<endl;
	int h=1;
	while(1){
	    if(!h&&p->coef>0.0)cout<<"+";
		h=0;
		cout<<p->coef;
		switch(p->exp){
		case 0:break;
		case 1:cout<<"X";break;
		default:cout<<"X^"<<p->exp;
		}
		p=p->link;
		if(p==NULL)break;
	}
	cout<<endl;
};
void Add(Polynomial&A,Polynomial&B,Polynomial&C){
    
	Term*pa,*pb,*pc,*p,*s;
	float temp;
	pc=C;
	pa=A->link;pb=B->link;
	while(pa!=NULL&&pb!=NULL){
	s=new Term;
	if(!s){cerr<<"存储分配失败!\n";exit(1);}
	if(pa->exp==pb->exp){
	
		temp=pa->coef+pb->coef;
		if(fabs(temp)>0.001)
		{
			pc->link=s;
			pc=pc->link;
			pc->coef=temp;
			pc->exp=pa->exp;
		}
		else delete s;
		pa=pa->link;
		pb=pb->link;
	}
	else if(pa->exp<pb->exp)
	{
			pc->link=s;
			pc=pc->link;
			pc->coef=pa->coef;
			pc->exp=pa->exp;
			pa=pa->link;

	}
	else{
	 
	     	pc->link=s;
			pc=pc->link;
			pc->coef=pb->coef;
			pc->exp=pb->exp;
			pb=pb->link;
	}
	}
	p=(pa)?pa:pb;
	while(p!=NULL){  
		pc->link=new Term;
	    if(!pc->link){cerr<<"存储分配失败!\n";exit(1);}
		pc=pc->link;
		pc->coef=p->coef;
		pc->exp=p->exp;
		p=p->link;
	
	}
	pc->link=NULL;
};
void Cut(Polynomial&A,Polynomial&B,Polynomial&C){
    
	Term*pa,*pb,*pc,*p,*s;
	float temp;
	pc=C;
	pa=A->link;pb=B->link;
	while(pa!=NULL&&pb!=NULL){
	s=new Term;
	if(!s){cerr<<"存储分配失败!\n";exit(1);}
	if(pa->exp==pb->exp){
	
		temp=pa->coef-pb->coef;
		if(temp!=0)
		{
			pc->link=s;
			pc=pc->link;
			pc->coef=temp;
			pc->exp=pa->exp;
		}
		else delete s;
		pa=pa->link;
		pb=pb->link;
	}
	else if(pa->exp<pb->exp)
	{
			pc->link=s;
			pc=pc->link;
			pc->coef=pa->coef;
			pc->exp=pa->exp;
			pa=pa->link;

	}
	else{
	 
	     	pc->link=s;
			pc=pc->link;
			pc->coef=0-pb->coef;
			pc->exp=pb->exp;
			pb=pb->link;
	}
	}
	if(pb==NULL)pc->link=pa;
	else{	pc->link=new Term;
	    if(!pc->link){cerr<<"存储分配失败!\n";exit(1);}
		pc=pc->link;
		pc->coef=0-pb->coef;
		pc->exp=pb->exp;
		pb=pb->link;
	}
	
};
void Mult(Polynomial&A,Polynomial&B,Polynomial&C)
{
	Term*pa,*pb,*pc;
	int AL,BL,i,k,maxExp;
	pc=C;
	for(pa=A;pa->link!=NULL;pa=pa->link);
	AL=pa->exp;
	for(pb=B;pb->link!=NULL;pb=pb->link);
	BL=pb->exp;
	if(AL!=-1||BL!=-1){
	     maxExp=AL+BL;
		 float*result=new float[maxExp+1];
		 if(!result){cerr<<"存储分配失败!\n";exit(1);}
		 for(i=0;i<=maxExp;i++)result[i]=0.0;
		 pa=A->link;
		 while(pa!=NULL){
		       pb=B->link;
			   while(pb!=NULL){
			         k=pa->exp+pb->exp;
					 result[k]=result[k]+pa->coef*pb->coef;
					 pb=pb->link;
			   }
			   pa=pa->link;
		 }
		 for(i=0;i<=maxExp;i++)
			 if(fabs(result[i])>0.001){
			    pc->link=new Term;
				if(!pc->link){cerr<<"存储分配失败!\n";exit(1);}
				pc=pc->link;pc->coef=result[i];pc->exp=i;
			 }
			 delete [] result; 
	}
	pc->link=NULL;
};
void main(){
	Term*A=new Term;
	A->link=NULL;A->coef=0;A->exp=0;
    Term*B=new Term;
	B->link=NULL;B->coef=0;B->exp=0;
	Term*C=new Term;
	char d;
	if(!B||!A||!C){cerr<<"存储分配失败!\n";exit(1);}
	cerr<<"请输入多项式A:\n";
	Input(A);
	cerr<<"请输入多项式B:\n";
	Input(B);
	cerr<<"请输入进行的运算（+，-，*，/):\n";
	cin>>d;
	switch(d){
		case '+':Add(A,B,C);Output(C);break;
		case '-':Cut(A,B,C);Output(C);break;
		case '*':Mult(A,B,C);Output(C);break;
		case '/':cerr<<"该功能待添加\n";break;
		default:cerr<<"您输入的运算符有误\n";
	}

}

