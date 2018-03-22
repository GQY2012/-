#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


// ͳ���ַ�Ƶ�ȵ���ʱ���
typedef struct {
	unsigned char uch;			// ��8bitsΪ��Ԫ���޷����ַ�
	unsigned long weight;		// ÿ�ࣨ�Զ����Ʊ������֣��ַ�����Ƶ��
} TmpNode;

typedef struct {
	unsigned char uch;
	unsigned int weight;
	char *code;
	unsigned int parent,lchild,rchild;
}HTNode,* HuffmanTree;
typedef char* *HuffmanCode;



void select(HuffmanTree HT,unsigned int n,int *s1,int *s2)
{
	// ����С�Ľ�� 
	int i;
	long min = ULONG_MAX;
	for(i = 0; i < n; i++)           
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s1 = i;
		}
		HT[*s1].parent = 1;   

	// �Ҵ�С�Ľ�� 
	min = ULONG_MAX;
	for(i = 0; i < n; i++)            
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s2 = i;
		}
} 

void HuffmanCoding(HuffmanTree &HT,HuffmanCode &HC,int *w,int n) 
{
	int m,i,c,f,start,s1,s2;
	HuffmanTree p,cd;
	if (n <= 1) return;
	m = 2 * n - 1;
	HT = (HuffmanTree)malloc((m+1) * sizeof(HTNode));
	for(p = HT,i = 1;i <= n;i++,p++,w++)	*p = {*w,0,0,0};
	for(;i <= m;i++,p++)	*p = {0,0,0,0};
	for(i = n + 1;i <= m;i++){
		select(HT,i - 1,s1,s2);
		HT[s1].parent = i, HT[s2].parent = i;
		HT[i].lchild = s1; HT[i].rchild = s2;
		HT[i].weight =HT[s1].weight + HT[s2].weight;
	}
	
	HC = (HuffmanCode)malloc((n+1)*sizeof(char*));
	cd = (char*)malloc(n*sizeof(char));
	cd[n-1]= "\0";
	for(i =1;i <= n;i++)
	{
		start = n-1;
		for(c = i,f = HT[i].parent;f != 0;c = f,f = HT[f].parent)
			if(HT[f.lchild == c])  cd[--start] = "0";
			else cd[--start] = "1";
			HC[i] = (char*)malloc((n-start)*sizeof(char));
			strcpy(HC[i],&cd[start]);
	}
			free (cd);
}
	
int compress(char *ifname, char *ofname)
{
	unsigned int i, j;
	unsigned int char_kinds;		// �ַ�����
	unsigned char char_temp;		// �ݴ�8bits�ַ�
	unsigned long file_len = 0;
	FILE *infile, *outfile;
	TmpNode node_temp;
	unsigned int node_num;
	HuffmanTree huf_tree;
	char code_buf[256] = "\0";		// ������뻺����
	unsigned int code_len;
	/*
	** ��̬����256����㣬�ݴ��ַ�Ƶ�ȣ�
	** ͳ�Ʋ������������������ͷ�
	*/
	TmpNode *tmp_nodes =(TmpNode *)malloc(256*sizeof(TmpNode));		

	// ��ʼ���ݴ���
	for(i = 0; i < 256; ++i)
	{
		tmp_nodes[i].weight = 0;
		tmp_nodes[i].uch = (unsigned char)i;		// �����256���±���256���ַ���Ӧ
	}

	// �����ļ�����ȡ�ַ�Ƶ��
	infile = fopen(ifname, "rb");
	// �ж������ļ��Ƿ����
	if (infile == NULL)
		return -1;
	fread((char *)&char_temp, sizeof(unsigned char), 1, infile);		// ����һ���ַ�
	while(!feof(infile))
	{
		++tmp_nodes[char_temp].weight;		// ͳ���±��Ӧ�ַ���Ȩ�أ����������������ʿ���ͳ���ַ�Ƶ��
		++file_len;
		fread((char *)&char_temp, sizeof(unsigned char), 1, infile);		// ����һ���ַ�
	}
	fclose(infile);

	// ���򣬽�Ƶ��Ϊ��ķ�����޳�
	for(i = 0; i < 256-1; ++i)           
		for(j = i+1; j < 256; ++j)
			if(tmp_nodes[i].weight < tmp_nodes[j].weight)
			{
				node_temp = tmp_nodes[i];
				tmp_nodes[i] = tmp_nodes[j];
				tmp_nodes[j] = node_temp;
			}

	// ͳ��ʵ�ʵ��ַ����ࣨ���ִ�����Ϊ0��
	for(i = 0; i < 256; ++i)
		if(tmp_nodes[i].weight == 0) 
			break;
	char_kinds = i;

	if (char_kinds == 1)
	{
		outfile = fopen(ofname, "wb");					// ��ѹ�������ɵ��ļ�
		fwrite((char *)&char_kinds, sizeof(unsigned int), 1, outfile);		// д���ַ�����
		fwrite((char *)&tmp_nodes[0].uch, sizeof(unsigned char), 1, outfile);		// д��Ψһ���ַ�
		fwrite((char *)&tmp_nodes[0].weight, sizeof(unsigned long), 1, outfile);		// д���ַ�Ƶ�ȣ�Ҳ�����ļ�����
		free(tmp_nodes);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * char_kinds - 1;		// �����ַ������������㽨������������������ 
		huf_tree = (HufNode *)malloc(node_num*sizeof(HufNode));		// ��̬������������������     

		// ��ʼ��ǰchar_kinds�����
		for(i = 0; i < char_kinds; ++i) 
		{ 
			// ���ݴ�����ַ���Ƶ�ȿ����������
			huf_tree[i].uch = tmp_nodes[i].uch; 
			huf_tree[i].weight = tmp_nodes[i].weight;
			huf_tree[i].parent = 0; 
		}	
		free(tmp_nodes); // �ͷ��ַ�Ƶ��ͳ�Ƶ��ݴ���

		// ��ʼ����node_num-char_kins�����
		for(; i < node_num; ++i) 
			huf_tree[i].parent = 0; 

		CreateTree(huf_tree, char_kinds, node_num);		// ������������

		HufCode(huf_tree, char_kinds);		// ���ɹ���������

		// д���ַ�����ӦȨ�أ�����ѹʱ�ؽ���������
		outfile = fopen(ofname, "wb");					// ��ѹ�������ɵ��ļ�
		fwrite((char *)&char_kinds, sizeof(unsigned int), 1, outfile);		// д���ַ�����
		for(i = 0; i < char_kinds; ++i)
		{
			fwrite((char *)&huf_tree[i].uch, sizeof(unsigned char), 1, outfile);			// д���ַ��������򣬶�����˳�򲻱䣩
			fwrite((char *)&huf_tree[i].weight, sizeof(unsigned long), 1, outfile);		// д���ַ���ӦȨ��
		}

		// �������ַ���Ȩ����Ϣ����д���ļ����Ⱥ��ַ�����
		fwrite((char *)&file_len, sizeof(unsigned long), 1, outfile);		// д���ļ�����
		infile = fopen(ifname, "rb");		// �Զ�������ʽ�򿪴�ѹ�����ļ�
		fread((char *)&char_temp, sizeof(unsigned char), 1, infile);     // ÿ�ζ�ȡ8bits
		while(!feof(infile))
		{
			// ƥ���ַ���Ӧ����
			for(i = 0; i < char_kinds; ++i)
				if(char_temp == huf_tree[i].uch)
					strcat(code_buf, huf_tree[i].code);

			// ��8λ��һ���ֽڳ��ȣ�Ϊ����Ԫ
			while(strlen(code_buf) >= 8)
			{
				char_temp = '\0';		// ����ַ��ݴ�ռ䣬��Ϊ�ݴ��ַ���Ӧ����
				for(i = 0; i < 8; ++i)
				{
					char_temp <<= 1;		// ����һλ��Ϊ��һ��bit�ڳ�λ��
					if(code_buf[i] == '1')
						char_temp |= 1;		// ������Ϊ"1"��ͨ���������������ӵ��ֽڵ����λ
				}
				fwrite((char *)&char_temp, sizeof(unsigned char), 1, outfile);		// ���ֽڶ�Ӧ��������ļ�
				strcpy(code_buf, code_buf+8);		// ���뻺��ȥ���Ѵ����ǰ��λ
			}
			fread((char *)&char_temp, sizeof(unsigned char), 1, infile);     // ÿ�ζ�ȡ8bits
		}
		// ���������8bits����
		code_len = strlen(code_buf);
		if(code_len > 0)
		{
			char_temp = '\0';		
			for(i = 0; i < code_len; ++i)
			{
				char_temp <<= 1;		
				if(code_buf[i] == '1')
					char_temp |= 1;
			}
			char_temp <<= 8-code_len;       // �������ֶδ�β���Ƶ��ֽڵĸ�λ
			fwrite((char *)&char_temp, sizeof(unsigned char), 1, outfile);       // �������һ���ֽ�
		}

		// �ر��ļ�
		fclose(infile);
		fclose(outfile);

		// �ͷ��ڴ�
		for(i = 0; i < char_kinds; ++i)
			free(huf_tree[i].code);
		free(huf_tree);
	}
}//compress



// ��ѹ����
int extract(char *ifname, char *ofname)
{
	unsigned int i;
	unsigned long file_len;
	unsigned long writen_len = 0;		// �����ļ�д�볤��
	FILE *infile, *outfile;
	unsigned int char_kinds;		// �洢�ַ�����
	unsigned int node_num;
	HuffmanTree huf_tree;
	unsigned char code_temp;		// �ݴ�8bits����
	unsigned int root;		// ������ڵ���������ƥ�����ʹ��

	infile = fopen(ifname, "rb");		// �Զ����Ʒ�ʽ��ѹ���ļ�
	// �ж������ļ��Ƿ����
	if (infile == NULL)
		return -1;

	// ��ȡѹ���ļ�ǰ�˵��ַ�����Ӧ���룬�����ؽ���������
	fread((char *)&char_kinds, sizeof(unsigned int), 1, infile);     // ��ȡ�ַ�������
	if (char_kinds == 1)
	{
		fread((char *)&code_temp, sizeof(unsigned char), 1, infile);     // ��ȡΨһ���ַ�
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);     // ��ȡ�ļ�����
		outfile = fopen(ofname, "wb");					// ��ѹ�������ɵ��ļ�
		while (file_len--)
			fwrite((char *)&code_temp, sizeof(unsigned char), 1, outfile);	
		fclose(infile);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * char_kinds - 1;		// �����ַ������������㽨������������������ 
		huf_tree = (HufNode *)malloc(node_num*sizeof(HufNode));		// ��̬��������������ռ�
		// ��ȡ�ַ�����ӦȨ�أ�������������ڵ�
		for(i = 0; i < char_kinds; ++i)     
		{
			fread((char *)&huf_tree[i].uch, sizeof(unsigned char), 1, infile);		// �����ַ�
			fread((char *)&huf_tree[i].weight, sizeof(unsigned long), 1, infile);	// �����ַ���ӦȨ��
			huf_tree[i].parent = 0;
		}
		// ��ʼ����node_num-char_kins������parent
		for(; i < node_num; ++i) 
			huf_tree[i].parent = 0;

		CreateTree(huf_tree, char_kinds, node_num);		// �ؽ�������������ѹ��ʱ��һ�£�

		// �����ַ���Ȩ����Ϣ�������Ŷ�ȡ�ļ����Ⱥͱ��룬���н���
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);	// �����ļ�����
		outfile = fopen(ofname, "wb");		// ��ѹ�������ɵ��ļ�
		root = node_num-1;
		while(1)
		{
			fread((char *)&code_temp, sizeof(unsigned char), 1, infile);		// ��ȡһ���ַ����ȵı���

			// �����ȡ��һ���ַ����ȵı��루ͨ��Ϊ8λ��
			for(i = 0; i < 8; ++i)
			{
				// �ɸ�����ֱ��Ҷ�ڵ�����ƥ������Ӧ�ַ�
				if(code_temp & 128)
					root = huf_tree[root].rchild;
				else
					root = huf_tree[root].lchild;

				if(root < char_kinds)
				{
					fwrite((char *)&huf_tree[root].uch, sizeof(unsigned char), 1, outfile);
					++writen_len;
					if (writen_len == file_len) break;		// �����ļ����ȣ������ڲ�ѭ��
					root = node_num-1;        // ��λΪ��������ƥ����һ���ַ�
				}
				code_temp <<= 1;		// �����뻺�����һλ�Ƶ����λ����ƥ��
			}
			if (writen_len == file_len) break;		// �����ļ����ȣ��������ѭ��
		}

		// �ر��ļ�
		fclose(infile);
		fclose(outfile);

		// �ͷ��ڴ�
		free(huf_tree);
	}
}//extract()

int main()
{
	while(1)
	{
		int opt, flag  = 0;		// ÿ�ν���ѭ����Ҫ��ʼ��flagΪ0
		char ifname[256], ofname[256];		// ������������ļ���
		// ������ѡ��������͵����ִ��ţ�1��ѹ����2����ѹ��3���˳�
	printf("**********************����������ѹ����************************\n\n");
	printf("\t\t��������1-ѹ�������ļ�\n");
	printf("\t\t��������2-��ѹ��ѹ���ļ�\n");
	printf("\t\t��������3-�˳�\n\n");
	printf("**************************************************************\n");
		scanf("%d", &opt);
		if (opt == 3)
			break;
		else
		{
			printf("����Ŀ���ļ�����");
			fflush(stdin);		// ��ձ�׼����������ֹ����gets������ȡ�ļ���
			gets(ifname);
			printf("����ļ����� ");
			fflush(stdin);
			gets(ofname);
		}
		switch(opt)
		{
		case 1: printf("ѹ����...\n");
				flag = compress(ifname, ofname);	// ѹ��������ֵ�����ж��Ƿ��ļ���������
				break;		
		case 2: printf("��ѹ��...\n");
				flag = extract(ifname, ofname);		// ��ѹ������ֵ�����ж��Ƿ��ļ���������
				break;		
		}
		if (flag == -1)
			printf("�ļ� \"%s\" �����ڣ�\n", ifname);		// �����־Ϊ'-1'�������ļ�������
		else
			printf("�������!\n");		// 
	}

	return 0;
}	

