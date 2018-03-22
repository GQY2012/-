#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 999999

// ͳ���ַ�Ƶ�ȵ���ʱ���
typedef struct {
	unsigned char uch;			// ��8bitsΪ��Ԫ���޷����ַ�
	unsigned long weight;		// ÿ�ࣨ�Զ����Ʊ������֣��ַ�����Ƶ��
} TmpNode;

// �����������
typedef struct {
	unsigned char uch;				// ��8bitsΪ��Ԫ���޷����ַ�
	unsigned long weight;			// ÿ�ࣨ�Զ����Ʊ������֣��ַ�����Ƶ��
	char *code;						// �ַ���Ӧ�Ĺ��������루��̬����洢�ռ䣩
	unsigned int parent, lchild, rchild;		// ����˫�׺����Һ���
} HTNode, *HuffmanTree;


void select(HTNode *HT, unsigned int n, int *s1, int *s2)
{
	// ��С
	unsigned int i;
	unsigned long min = MAX;
	for(i = 0; i < n; i++)           
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s1 = i;
		}
		HT[*s1].parent=1;   // ��Ǵ˽���ѱ�ѡ��

	// ��С
	min = MAX;
	for(i = 0; i < n; i++)            
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s2 = i;
		}
} 

// ������������
void CreateTree(HTNode *HT, unsigned int n, unsigned int node_num)
{
	unsigned int i;
	int s1, s2;
	for(i = n; i < node_num; i++)  
	{ 
		select(HT, i, &s1, &s2);		// ѡ����С���������
		HT[s1].parent = HT[s2].parent = i; 
		HT[i].lchild = s1; 
		HT[i].rchild = s2; 
		HT[i].weight = HT[s1].weight + HT[s2].weight; 
	} 
}

// ���ɹ���������
void HuffmanCode(HTNode *HT, unsigned n)
{
	unsigned int i;
	int c, f, start;
	char *code_tmp = (char *)malloc(256*sizeof(char));		// �ݴ���룬���256��Ҷ�ӣ����볤�Ȳ�����255
	code_tmp[255] = '\0'; 

	for(i = 0; i < n; i++) 
	{
		start = 255;	

		// ��Ҷ�����������������
		for(c = i, f = HT[i].parent; f != 0; 
			c = f, f = HT[f].parent)  
			if(HT[f].lchild == c) 
				code_tmp[--start] = '0';
			else 
				code_tmp[--start] = '1';
		HT[i].code = (char *)malloc((256-start)*sizeof(char));			 
		strcpy(HT[i].code, &code_tmp[start]);    
	} 
	free(code_tmp);	
}


int compress(char *inname, char *outname)
{
	unsigned int i, j;
	unsigned int n;		// �ַ�����
	unsigned char char_temp;		// �ݴ�8bits�ַ�
	unsigned long file_len = 0;
	FILE *infile, *outfile;
	TmpNode node_temp;
	unsigned int node_num;
	HuffmanTree HT;
	char code_buf[256] = "\0";		// ������뻺����
	unsigned int code_len;
	TmpNode *tmp_nodes =(TmpNode *)malloc(256*sizeof(TmpNode));		

	// ��ʼ���ݴ���
	for(i = 0; i < 256; i++)
	{
		tmp_nodes[i].weight = 0;
		tmp_nodes[i].uch = (unsigned char)i;		// �����256���±���256���ַ���Ӧ
	}

	// �����ļ�����ȡ�ַ�Ƶ��
	infile = fopen(inname, "rb");
	// �ж������ļ��Ƿ����
	if (infile == NULL)
		return -1;
	fread((char *)&char_temp, sizeof(unsigned char), 1, infile);		// ����һ���ַ�
	while(!feof(infile))
	{
		tmp_nodes[char_temp].weight++;		// ͳ���±��Ӧ�ַ���Ȩ�أ����������������ʿ���ͳ���ַ�Ƶ��
		file_len++;
		fread((char *)&char_temp, sizeof(unsigned char), 1, infile);		// ����һ���ַ�
	}
	fclose(infile);

	// ���򣬽�Ƶ��Ϊ��ķ�����޳�
	for(i = 0; i < 256-1; i++)           
		for(j = i+1; j < 256; j++)
			if(tmp_nodes[i].weight < tmp_nodes[j].weight)
			{
				node_temp = tmp_nodes[i];
				tmp_nodes[i] = tmp_nodes[j];
				tmp_nodes[j] = node_temp;
			}

	// ͳ��ʵ�ʵ��ַ�����
	for(i = 0; i < 256; i++)
		if(tmp_nodes[i].weight == 0) 
			break;
	n = i;

	if (n == 1)
	{
		outfile = fopen(outname, "wb");					// ��ѹ�������ɵ��ļ�
		fwrite((char *)&n, sizeof(unsigned int), 1, outfile);		// д���ַ�����
		fwrite((char *)&tmp_nodes[0].uch, sizeof(unsigned char), 1, outfile);		// д��Ψһ���ַ�
		fwrite((char *)&tmp_nodes[0].weight, sizeof(unsigned long), 1, outfile);		// д���ַ�Ƶ�ȣ�Ҳ�����ļ�����
		free(tmp_nodes);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * n - 1;		// �����ַ������������㽨������������������ 
		HT = (HTNode *)malloc(node_num*sizeof(HTNode));		     

		// ��ʼ��ǰn�����
		for(i = 0; i < n; i++) 
		{ 
			// ���ݴ�����ַ���Ƶ�ȿ����������
			HT[i].uch = tmp_nodes[i].uch; 
			HT[i].weight = tmp_nodes[i].weight;
			HT[i].parent = 0; 
		}	
		free(tmp_nodes); // �ͷ��ַ�Ƶ��ͳ�Ƶ��ݴ���

		// ��ʼ����node_num-char_kins�����
		for(; i < node_num; i++) 
			HT[i].parent = 0; 

		CreateTree(HT, n, node_num);		// ������������

		HuffmanCode(HT, n);		// ���ɹ���������

		// д���ַ�����ӦȨ�أ�����ѹʱ�ؽ���������
		outfile = fopen(outname, "wb");					// ��ѹ�������ɵ��ļ�
		fwrite((char *)&n, sizeof(unsigned int), 1, outfile);		// д���ַ�����
		for(i = 0; i < n; i++)
		{
			fwrite((char *)&HT[i].uch, sizeof(unsigned char), 1, outfile);			// д���ַ��������򣬶�����˳�򲻱䣩
			fwrite((char *)&HT[i].weight, sizeof(unsigned long), 1, outfile);		// д���ַ���ӦȨ��
		}

		// �������ַ���Ȩ����Ϣ����д���ļ����Ⱥ��ַ�����
		fwrite((char *)&file_len, sizeof(unsigned long), 1, outfile);		// д���ļ�����
		infile = fopen(inname, "rb");		// �Զ�������ʽ�򿪴�ѹ�����ļ�
		fread((char *)&char_temp, sizeof(unsigned char), 1, infile);     // ÿ�ζ�ȡ8bits
		while(!feof(infile))
		{
			// ƥ���ַ���Ӧ����
			for(i = 0; i < n; i++)
				if(char_temp == HT[i].uch)
					strcat(code_buf, HT[i].code);

			// ��8λ��һ���ֽڳ��ȣ�Ϊ����Ԫ
			while(strlen(code_buf) >= 8)
			{
				char_temp = '\0';		// ����ַ��ݴ�ռ䣬��Ϊ�ݴ��ַ���Ӧ����
				for(i = 0; i < 8; i++)
				{
					char_temp <<= 1;		// ����һλ��Ϊ��һ��bit�ڳ�λ��
					if(code_buf[i] == '1')
						char_temp = char_temp | 1;		// ������Ϊ"1"��ͨ���������������ӵ��ֽڵ����λ
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
			for(i = 0; i < code_len; i++)
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
		for(i = 0; i < n; i++)
			free(HT[i].code);
		free(HT);
	}
}//compress



// ��ѹ����
int extract(char *inname, char *outname)
{
	unsigned int i;
	unsigned long file_len;
	unsigned long writen_len = 0;		// �����ļ�д�볤��
	FILE *infile, *outfile;
	unsigned int n;		// �洢�ַ�����
	unsigned int node_num;
	HuffmanTree HT;
	unsigned char code_temp;		// �ݴ�8bits����
	unsigned int root;		// ������ڵ���������ƥ�����ʹ��

	infile = fopen(inname, "rb");		// �Զ����Ʒ�ʽ��ѹ���ļ�
	// �ж������ļ��Ƿ����
	if (infile == NULL)
		return -1;

	// ��ȡѹ���ļ�ǰ�˵��ַ�����Ӧ���룬�����ؽ���������
	fread((char *)&n, sizeof(unsigned int), 1, infile);     // ��ȡ�ַ�������
	if (n == 1)
	{
		fread((char *)&code_temp, sizeof(unsigned char), 1, infile);     // ��ȡΨһ���ַ�
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);     // ��ȡ�ļ�����
		outfile = fopen(outname, "wb");					// ��ѹ�������ɵ��ļ�
		while (file_len--)
			fwrite((char *)&code_temp, sizeof(unsigned char), 1, outfile);	
		fclose(infile);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * n - 1;		// �����ַ������������㽨������������������ 
		HT = (HTNode *)malloc(node_num*sizeof(HTNode));		// ��̬��������������ռ�
		// ��ȡ�ַ�����ӦȨ�أ�������������ڵ�
		for(i = 0; i < n; i++)     
		{
			fread((char *)&HT[i].uch, sizeof(unsigned char), 1, infile);		// �����ַ�
			fread((char *)&HT[i].weight, sizeof(unsigned long), 1, infile);	// �����ַ���ӦȨ��
			HT[i].parent = 0;
		}
		// ��ʼ����node_num-char_kins������parent
		for(; i < node_num; i++) 
			HT[i].parent = 0;

		CreateTree(HT, n, node_num);		// �ؽ�������������ѹ��ʱ��һ�£�

		// �����ַ���Ȩ����Ϣ�������Ŷ�ȡ�ļ����Ⱥͱ��룬���н���
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);	// �����ļ�����
		outfile = fopen(outname, "wb");		// ��ѹ�������ɵ��ļ�
		root = node_num-1;
		while(1)
		{
			fread((char *)&code_temp, sizeof(unsigned char), 1, infile);		// ��ȡһ���ַ����ȵı���

			// �����ȡ��һ���ַ����ȵı��루ͨ��Ϊ8λ��
			for(i = 0; i < 8; i++)
			{
				// �ɸ�����ֱ��Ҷ�ڵ�����ƥ������Ӧ�ַ�
				if(code_temp & 128)
					root = HT[root].rchild;
				else
					root = HT[root].lchild;

				if(root < n)
				{
					fwrite((char *)&HT[root].uch, sizeof(unsigned char), 1, outfile);
					++writen_len;
					if (writen_len == file_len) break;		// �����ļ����ȣ������ڲ�ѭ��
					root = node_num-1;        // ��λΪ��������ƥ����һ���ַ�
				}
				code_temp <<= 1;		// �����뻺�����һλ�Ƶ����λ����ƥ��
			}
			if (writen_len == file_len) break;		// �����ļ����ȣ��������ѭ��
		}
		fclose(infile);
		fclose(outfile);
		free(HT);
	}
}

int main()
{
	while(1)
	{
		int m, flag  = 0;		
		char inname[256], outname[256];		
	printf("**********************����������ѹ����************************\n\n");
	printf("\t\t��������1-ѹ�������ļ�\n");
	printf("\t\t��������2-��ѹ��ѹ���ļ�\n");
	printf("\t\t��������3-����\n");
	printf("\t\t��������4-�˳�\n\n");
	printf("**************************************************************\n");
		scanf("%d", &m);
		if (m == 4)
			break;
		else if(m == 3)
		system ("cls");
		else
		{
			printf("����Ŀ���ļ�����");
			gets(inname);
			printf("����ļ����� ");
			gets(outname);
		}
		switch(m)
		{
		case 1: printf("ѹ����...\n");
				flag = compress(inname, outname);	
				break;		
		case 2: printf("��ѹ��...\n");
				flag = extract(inname, outname);		
				break;;	
		}
		if (flag == -1)
			printf("�ļ� \"%s\" �����ڣ�\n", inname);		
		else
			printf("�������!\n");		// 
	}

	return 0;
}

