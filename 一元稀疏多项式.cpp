#include"stdlib.h"
#include"stdio.h"
#include"math.h"
typedef struct term { //��ı�ʾ������ʽ������ΪLinkList������Ԫ��
	float coef; //ϵ��
	int expn; //ָ��
	struct term *next;
	}term;

term* CreatPolyn(term *P,int m) {
	// ����m���ϵ����ָ����������ʾһԪ����ʽ����������P
	if(m <= 0) return NULL;
	term *h = P = (term*)malloc(sizeof(term)), *q;
	P->coef = 0.0;
	int i;
	printf("��������%d������ǰһ��Ϊϵ������һ��Ϊָ����\n",m*2);
	for (i = 1; i <= m; i++) { // ��������m��������
		scanf("%f%d",&P->coef,&P->expn);
		q = P;
	P = P->next = (term*)malloc(sizeof(term));
	}
	q->next = NULL;
	free(P);
	return h;
}

term* selsort(term *h) { //����
	term *g, *p, *q;
	if(!h) return NULL;
		float f;
	int i, fini = 1;
	for(g = h;g->next&&fini;g = g->next) {
	fini = 0;
	for(p = h,q = h->next;q;p = p->next,q = q->next)
	if (p->expn < q->expn) {
		f = p->coef;i = p->expn;
		p->coef = q->coef;p->expn = q->expn;
		q->coef = f;q->expn = i;
		fini = 1;
	}
}
    for(g = h,p = g->next;p;)
	if(g->expn==p->expn) {
		g->coef += p->coef;
		g->next = p->next;
		q = p;
		p = p->next;
		free(q);
	}
	else if(g->next) {
		g = g->next;
		p = p->next;
	}
return h;
}

void PrintfPoly(term *P) {
	term *q = P;
	if(!q) {
		putchar('0');
	return;
}
	if(q->coef!=1) {
		printf("%g",q->coef);
	if(q->expn==1) putchar('X');
	else if(q->expn) printf("X^%d",q->expn);
}
	else if(!q->expn) putchar('1');
	else if(q->expn==1) putchar('X');
	else printf("X^%d",q->expn);
	q = q->next;
	while (q) {
		if(q->coef > 0) putchar('+');
		if(q->coef!=1) {
			printf("%g",q->coef);
		if(q->expn==1) putchar('X');
		else if(q->expn) printf("X^%d",q->expn);
		}
		else if(!q->expn) putchar('1');
		else if(q->expn==1) putchar('X');
		else printf("X^%d",q->expn);
			q = q->next;
		}
}


int Compare(term *a, term *b) {
 	if (a->expn < b->expn) return -1;
	if (a->expn > b->expn) return 1;
return 0;
}

term* APolyn(term *Pa, term *Pb) {
// ����ʽ�ӷ���Pa = Pa��Pb��������������ʽ�Ľ�㹹��"�Ͷ���ʽ"��
	term *h, *qa = Pa, *qb = Pb, *p, *q;
	float sum;
	h = p = (term*)malloc(sizeof(term));
	p->next = NULL;
while (qa && qb) { // Pa��Pb���ǿ�
	switch (Compare(qa,qb)) {
	case -1: // ����ʽPA�е�ǰ����ָ��ֵС
	p->next = qb;
	p = qb;
	qb = qb->next;
	break;
	case 0: // ���ߵ�ָ��ֵ���
	sum = qa->coef + qb->coef;
if (sum != 0.0) { // �޸Ķ���ʽPA�е�ǰ����ϵ��ֵ
	p->next = qa;
	qa->coef = sum;
	p = qa;
	qa = qa->next;
}
else { // ɾ������ʽPA�е�ǰ���
	q = qa;
	qa = qa->next;
	free(q);
}
	q = qb;
	qb = qb->next;
	free(q); break;
	case 1: // ����ʽPB�е�ǰ����ָ��ֵС
	p->next = qa;
	p = qa;
	qa = qa->next; break;
}
}
	if (qa) p->next = qa; // ����Pa��ʣ����
	if (qb) p->next = qb; // ����Pb��ʣ����
	q = h;
	h = h->next;
	free(q);
	return h;
}

term* A(term *Pa, term *Pb) {
	int n;
	puts("����ڶ���һԪ����ʽ������");
	scanf("%d",&n);
	Pb = CreatPolyn(Pb,n);
	Pb = selsort(Pb);
	PrintfPoly(Pa);
	if(Pb && Pb->coef>0) printf(" + ");
		PrintfPoly(Pb);
		Pa = APolyn(Pa,Pb);
		printf(" = ");
		Pa = selsort(Pa);
		PrintfPoly(Pa);
return Pa;
}

term* BPolyn(term *Pa, term *Pb) {  // ����
	term *p = Pb;
	while(p) {
	p->coef *= -1;
	p = p->next;
	}
	return APolyn(Pa,Pb);
}

term* B(term *Pa, term *Pb) {
	int n;
	puts("������һ��һԪ����ʽ������");
	scanf("%d",&n);
	Pb = CreatPolyn(Pb,n);
	Pb = selsort(Pb);
	PrintfPoly(Pa);
	printf(" - ");
		putchar('(');PrintfPoly(Pb);putchar(')');
	Pa = BPolyn(Pa,Pb);
	printf(" = ");
	Pa = selsort(Pa);
	PrintfPoly(Pa);
return Pa;
}


float GetPoly(term*p,float x)//һԪ����ʽ��x����ֵ
 {
 	term*q=p;
 	float n=0;
 	while(q)
 	{
 		n=n+((q->coef)*pow(x,q->expn));
 		q=q->next;
	 }
	 return n;
}

 term* Derivation(term*p)//��
 {
 	term*q=p;
	 while(q)
	 {
	 	if(q->expn==0)
	 	q->coef=0;
	 	else
	 	{
	 		q->coef=(q->coef)*(q->expn);
	 		(q->expn)--;
		 }
		 q=q->next;
	 }
	 return p;
 }


main() {
	term *M=NULL,*N=NULL;
	int m=1,n;
	puts("һԪ����ʽ����:\n�����һ��һԪ����ʽ������");
	scanf("%d",&n);
	M = CreatPolyn(M,n);
	M = selsort(M);
	PrintfPoly(M);
while(m){
	printf("\n1:��\n2:��\n3:��ֵ\n4:��\n0:����\n");
scanf("%d",&m);
	switch(m) {
		case 1:M = A(M,N); break;
		case 2:M = B(M,N); break;
		case 3:
 				printf("����x =\n");
 				float x;
 				scanf("%f",&x);
 				printf("����ʽֵΪ%g\n",GetPoly(M,x));
				break;
		case 4:
				PrintfPoly(Derivation(M));
				break;
		case 0: m=0; break;
		}
	}
}


