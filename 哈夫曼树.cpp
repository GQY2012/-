﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 999999

// 统计字符频度的临时结点
typedef struct {
	unsigned char uch;			// 以8bits为单元的无符号字符
	unsigned long weight;		// 每类（以二进制编码区分）字符出现频度
} TmpNode;

// 哈夫曼树结点
typedef struct {
	unsigned char uch;				// 以8bits为单元的无符号字符
	unsigned long weight;			// 每类（以二进制编码区分）字符出现频度
	char *code;						// 字符对应的哈夫曼编码（动态分配存储空间）
	unsigned int parent, lchild, rchild;		// 定义双亲和左右孩子
} HTNode, *HuffmanTree;


void select(HTNode *HT, unsigned int n, int *s1, int *s2)
{
	// 最小
	unsigned int i;
	unsigned long min = MAX;
	for(i = 0; i < n; i++)           
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s1 = i;
		}
		HT[*s1].parent=1;   // 标记此结点已被选中

	// 次小
	min = MAX;
	for(i = 0; i < n; i++)            
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s2 = i;
		}
} 

// 建立哈夫曼树
void CreateTree(HTNode *HT, unsigned int n, unsigned int node_num)
{
	unsigned int i;
	int s1, s2;
	for(i = n; i < node_num; i++)  
	{ 
		select(HT, i, &s1, &s2);		// 选择最小的两个结点
		HT[s1].parent = HT[s2].parent = i; 
		HT[i].lchild = s1; 
		HT[i].rchild = s2; 
		HT[i].weight = HT[s1].weight + HT[s2].weight; 
	} 
}

// 生成哈夫曼编码
void HuffmanCode(HTNode *HT, unsigned n)
{
	unsigned int i;
	int c, f, start;
	char *code_tmp = (char *)malloc(256*sizeof(char));		// 暂存编码，最多256个叶子，编码长度不超多255
	code_tmp[255] = '\0'; 

	for(i = 0; i < n; i++) 
	{
		start = 255;	

		// 从叶子向根反向遍历求编码
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
	unsigned int n;		// 字符种类
	unsigned char char_temp;		// 暂存8bits字符
	unsigned long file_len = 0;
	FILE *infile, *outfile;
	TmpNode node_temp;
	unsigned int node_num;
	HuffmanTree HT;
	char code_buf[256] = "\0";		// 待存编码缓冲区
	unsigned int code_len;
	TmpNode *tmp_nodes =(TmpNode *)malloc(256*sizeof(TmpNode));		

	// 初始化暂存结点
	for(i = 0; i < 256; i++)
	{
		tmp_nodes[i].weight = 0;
		tmp_nodes[i].uch = (unsigned char)i;		// 数组的256个下标与256种字符对应
	}

	// 遍历文件，获取字符频度
	infile = fopen(inname, "rb");
	// 判断输入文件是否存在
	if (infile == NULL)
		return -1;
	fread((char *)&char_temp, sizeof(unsigned char), 1, infile);		// 读入一个字符
	while(!feof(infile))
	{
		tmp_nodes[char_temp].weight++;		// 统计下标对应字符的权重，利用数组的随机访问快速统计字符频度
		file_len++;
		fread((char *)&char_temp, sizeof(unsigned char), 1, infile);		// 读入一个字符
	}
	fclose(infile);

	// 排序，将频度为零的放最后，剔除
	for(i = 0; i < 256-1; i++)           
		for(j = i+1; j < 256; j++)
			if(tmp_nodes[i].weight < tmp_nodes[j].weight)
			{
				node_temp = tmp_nodes[i];
				tmp_nodes[i] = tmp_nodes[j];
				tmp_nodes[j] = node_temp;
			}

	// 统计实际的字符种类
	for(i = 0; i < 256; i++)
		if(tmp_nodes[i].weight == 0) 
			break;
	n = i;

	if (n == 1)
	{
		outfile = fopen(outname, "wb");					// 打开压缩后将生成的文件
		fwrite((char *)&n, sizeof(unsigned int), 1, outfile);		// 写入字符种类
		fwrite((char *)&tmp_nodes[0].uch, sizeof(unsigned char), 1, outfile);		// 写入唯一的字符
		fwrite((char *)&tmp_nodes[0].weight, sizeof(unsigned long), 1, outfile);		// 写入字符频度，也就是文件长度
		free(tmp_nodes);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * n - 1;		// 根据字符种类数，计算建立哈夫曼树所需结点数 
		HT = (HTNode *)malloc(node_num*sizeof(HTNode));		     

		// 初始化前n个结点
		for(i = 0; i < n; i++) 
		{ 
			// 将暂存结点的字符和频度拷贝到树结点
			HT[i].uch = tmp_nodes[i].uch; 
			HT[i].weight = tmp_nodes[i].weight;
			HT[i].parent = 0; 
		}	
		free(tmp_nodes); // 释放字符频度统计的暂存区

		// 初始化后node_num-char_kins个结点
		for(; i < node_num; i++) 
			HT[i].parent = 0; 

		CreateTree(HT, n, node_num);		// 创建哈夫曼树

		HuffmanCode(HT, n);		// 生成哈夫曼编码

		// 写入字符和相应权重，供解压时重建哈夫曼树
		outfile = fopen(outname, "wb");					// 打开压缩后将生成的文件
		fwrite((char *)&n, sizeof(unsigned int), 1, outfile);		// 写入字符种类
		for(i = 0; i < n; i++)
		{
			fwrite((char *)&HT[i].uch, sizeof(unsigned char), 1, outfile);			// 写入字符（已排序，读出后顺序不变）
			fwrite((char *)&HT[i].weight, sizeof(unsigned long), 1, outfile);		// 写入字符对应权重
		}

		// 紧接着字符和权重信息后面写入文件长度和字符编码
		fwrite((char *)&file_len, sizeof(unsigned long), 1, outfile);		// 写入文件长度
		infile = fopen(inname, "rb");		// 以二进制形式打开待压缩的文件
		fread((char *)&char_temp, sizeof(unsigned char), 1, infile);     // 每次读取8bits
		while(!feof(infile))
		{
			// 匹配字符对应编码
			for(i = 0; i < n; i++)
				if(char_temp == HT[i].uch)
					strcat(code_buf, HT[i].code);

			// 以8位（一个字节长度）为处理单元
			while(strlen(code_buf) >= 8)
			{
				char_temp = '\0';		// 清空字符暂存空间，改为暂存字符对应编码
				for(i = 0; i < 8; i++)
				{
					char_temp <<= 1;		// 左移一位，为下一个bit腾出位置
					if(code_buf[i] == '1')
						char_temp = char_temp | 1;		// 当编码为"1"，通过或操作符将其添加到字节的最低位
				}
				fwrite((char *)&char_temp, sizeof(unsigned char), 1, outfile);		// 将字节对应编码存入文件
				strcpy(code_buf, code_buf+8);		// 编码缓存去除已处理的前八位
			}
			fread((char *)&char_temp, sizeof(unsigned char), 1, infile);     // 每次读取8bits
		}
		// 处理最后不足8bits编码
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
			char_temp <<= 8-code_len;       // 将编码字段从尾部移到字节的高位
			fwrite((char *)&char_temp, sizeof(unsigned char), 1, outfile);       // 存入最后一个字节
		}

		// 关闭文件
		fclose(infile);
		fclose(outfile);

		// 释放内存
		for(i = 0; i < n; i++)
			free(HT[i].code);
		free(HT);
	}
}//compress



// 解压函数
int extract(char *inname, char *outname)
{
	unsigned int i;
	unsigned long file_len;
	unsigned long writen_len = 0;		// 控制文件写入长度
	FILE *infile, *outfile;
	unsigned int n;		// 存储字符种类
	unsigned int node_num;
	HuffmanTree HT;
	unsigned char code_temp;		// 暂存8bits编码
	unsigned int root;		// 保存根节点索引，供匹配编码使用

	infile = fopen(inname, "rb");		// 以二进制方式打开压缩文件
	// 判断输入文件是否存在
	if (infile == NULL)
		return -1;

	// 读取压缩文件前端的字符及对应编码，用于重建哈夫曼树
	fread((char *)&n, sizeof(unsigned int), 1, infile);     // 读取字符种类数
	if (n == 1)
	{
		fread((char *)&code_temp, sizeof(unsigned char), 1, infile);     // 读取唯一的字符
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);     // 读取文件长度
		outfile = fopen(outname, "wb");					// 打开压缩后将生成的文件
		while (file_len--)
			fwrite((char *)&code_temp, sizeof(unsigned char), 1, outfile);	
		fclose(infile);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * n - 1;		// 根据字符种类数，计算建立哈夫曼树所需结点数 
		HT = (HTNode *)malloc(node_num*sizeof(HTNode));		// 动态分配哈夫曼树结点空间
		// 读取字符及对应权重，存入哈夫曼树节点
		for(i = 0; i < n; i++)     
		{
			fread((char *)&HT[i].uch, sizeof(unsigned char), 1, infile);		// 读入字符
			fread((char *)&HT[i].weight, sizeof(unsigned long), 1, infile);	// 读入字符对应权重
			HT[i].parent = 0;
		}
		// 初始化后node_num-char_kins个结点的parent
		for(; i < node_num; i++) 
			HT[i].parent = 0;

		CreateTree(HT, n, node_num);		// 重建哈夫曼树（与压缩时的一致）

		// 读完字符和权重信息，紧接着读取文件长度和编码，进行解码
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);	// 读入文件长度
		outfile = fopen(outname, "wb");		// 打开压缩后将生成的文件
		root = node_num-1;
		while(1)
		{
			fread((char *)&code_temp, sizeof(unsigned char), 1, infile);		// 读取一个字符长度的编码

			// 处理读取的一个字符长度的编码（通常为8位）
			for(i = 0; i < 8; i++)
			{
				// 由根向下直至叶节点正向匹配编码对应字符
				if(code_temp & 128)
					root = HT[root].rchild;
				else
					root = HT[root].lchild;

				if(root < n)
				{
					fwrite((char *)&HT[root].uch, sizeof(unsigned char), 1, outfile);
					++writen_len;
					if (writen_len == file_len) break;		// 控制文件长度，跳出内层循环
					root = node_num-1;        // 复位为根索引，匹配下一个字符
				}
				code_temp <<= 1;		// 将编码缓存的下一位移到最高位，供匹配
			}
			if (writen_len == file_len) break;		// 控制文件长度，跳出外层循环
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
	printf("**********************哈夫曼编码压缩器************************\n\n");
	printf("\t\t输入数字1-压缩任意文件\n");
	printf("\t\t输入数字2-解压已压缩文件\n");
	printf("\t\t输入数字3-清屏\n");
	printf("\t\t输入数字4-退出\n\n");
	printf("**************************************************************\n");
		scanf("%d", &m);
		if (m == 4)
			break;
		else if(m == 3)
		system ("cls");
		else
		{
			printf("输入目标文件名：");
			gets(inname);
			printf("输出文件名： ");
			gets(outname);
		}
		switch(m)
		{
		case 1: printf("压缩中...\n");
				flag = compress(inname, outname);	
				break;		
		case 2: printf("解压中...\n");
				flag = extract(inname, outname);		
				break;;	
		}
		if (flag == -1)
			printf("文件 \"%s\" 不存在！\n", inname);		
		else
			printf("操作完成!\n");		// 
	}

	return 0;
}

