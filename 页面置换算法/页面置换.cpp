// ҳ���û�.cpp : �������̨Ӧ�ó������ڵ㡣
//
// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include"stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct item
{
	int num;         //ҳ��
	int time;        //�ȴ�ʱ��
	int M;         //�޸�λ
	int F;         //����λ
}Pro;
int curmemory;         //���������е�ҳ�����
int missNum;           //ȱҳ����
float missRate;        //ȱҳ��
int pageNum;           //ϵͳ�������ҵ�������е�ҳ����
int memoryNum;         //�����ڴ�ҳ����
int nru[200] = { 0 };

Pro  page[100000];             //��ҵҳ�漯
Pro  memory[20];          //�ڴ�ҳ�漯

void print(Pro *memory1)//��ӡ��ǰ��ҳ��
{
	int i;
	for (i = 0; i<memoryNum; i++)
	{
		printf("%d ", memory1[i].num);
	}
	printf("\n");
}

void initMemory() {
	int i;
	for (i = 0; i<memoryNum; i++)        //��ʼ���ڴ���ҳ��
	{
		memory[i].num = -1;                //ҳ��Ϊ����-1��ʾ
		memory[i].time = -1;
		memory[i].M = 0;                   //�޸�λ��ʼ��Ϊ0
		memory[i].F = 0;					//����λ��ʼ��
	}
}
int  Search(int num1, Pro *memory1)
{
	int j;
	for (j = 0; j<memoryNum; j++)
	{
		if (num1 == memory1[j].num)
			return j;
	}
	return -1;
}
int Max(Pro *memory1)//Ѱ������δʹ�õ�ҳ��
{
	int max = 0;
	for (int k = 1; k<memoryNum; k++)
	{
		if (memory1[k].time > memory1[max].time)
			max = k;
	}
	return max;
}
void creatPage() {
	int i;
	printf("����ϵͳ�������ҵ�������е�ҳ����:");
	scanf_s("%d", &pageNum);
	printf("�����ڴ�ҳ����:");
	scanf_s("%d", &memoryNum);
	initMemory();
	srand((unsigned int)time(NULL));
	for (i = 0; i < pageNum; i++) {
		page[i].num = rand() % 10;
	}
}
int getS(int a, int b) {
	if (a == 0 && b == 0)
		return 4;
	else if (a == 0 && b == 1)
		return 3;
	else if (a == 1 && b == 0)
		return 2;
	else if (a == 1 && b == 1)
		return 1;
	else
		return 0;
}

int optimal(int num, int tag, Pro *memory1, Pro *page1)//����û��㷨
{
	int k, j, min[20];
	for (k = 0; k<memoryNum; k++)//k<�����ڴ�ҳ����
		min[k] = 500;
	for (k = 0; k<memoryNum; k++)
	{
		j = tag;//��¼�������ٴκ󱻷���
		do {

			j++;
			if (j>pageNum)
				break;
		} while (page1[j].num != memory1[k].num);//ҳ���ٴα�����ʱѭ������
		if (j<500)
		{

			min[k] = j;//���ҳ����һ�α����ʵĴ���
		}
	}
	int max = 0;
	for (int t = 1; t<memoryNum; t++)
	{
		if (min[t]>min[max])//�ж���һ��ҳ�����δ��ʱ���ڲ��ٱ����ʵ�
			max = t;
	}
	return max;
}
void OPT()
{
	int i;
	missNum = 0;
	curmemory = 0;
	printf("����û��㷨�û����:   \n");
	for (i = 0; i<pageNum; i++)
	{
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory)<0)//����������û���ҵ���ҳ��
		{
			if (i<memoryNum)//i<�����ڴ�ҳ����
				curmemory = i;
			else
			{
				curmemory = optimal(page[i].num, i, memory, page);
				missNum++;//ȱҳ����+1
				//printf("*");
			}
			memory[curmemory].num = page[i].num;//�ѵ��������ĵ�ǰҳ�Ž��ڴ���
			//print(memory);
		}
		else
		{
			/*print(memory);*/
		}
	}//end for
	missRate = (float)missNum / pageNum;
	printf("����û��㷨��ȱҳ����Ϊ��%d   ȱҳ��Ϊ:  %.2f%%\n", missNum, missRate * 100);

}
void Random(){
	int i;
	missNum = 0;
	curmemory = 0;
	printf("����û��㷨�û����:   \n");
	for (i = 0; i < pageNum; i++){
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory) < 0)//����������û���ҵ���ҳ��
		{
			if (i<memoryNum)//i<�����ڴ�ҳ����
				curmemory = i;
			else
			{
				curmemory = rand() % memoryNum;
				missNum++;//ȱҳ����+1
				//printf("*");
			}
			memory[curmemory].num = page[i].num;//�ѵ��������ĵ�ǰҳ�Ž��ڴ���
			//print(memory);
		}
		else
		{
			//print(memory);
		}
	}
	missRate = (float)missNum / pageNum;
	printf("����û��㷨��ȱҳ����Ϊ��%d   ȱҳ��Ϊ:  %.2f%%\n", missNum, missRate * 100);

}
void FIFO()
{
	int i;
	int time[20];
	int max;
	for (i = 0; i < memoryNum; i++)
	{
		time[i] = 0;
	}
	missNum = 0;
	curmemory = 0;
	printf("FIFOҳ���û����:   \n");
	for (i = 0; i<pageNum; i++)
	{
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory) < 0)//����������û���ҵ���ҳ��
		{
			if (i<memoryNum)//i<�����ڴ�ҳ����
				curmemory = i;
			else
			{
				max = 0;
				for (int j = 1; j < memoryNum; j++)
				{
					if (time[j]>time[max])max = j;
				}
				curmemory = max;
				missNum++;//ȱҳ����+1
				//printf("*");
				time[curmemory] = 0;
			}
			memory[curmemory].num = page[i].num;//�ѵ��������ĵ�ǰҳ�Ž��ڴ���
			//print(memory);
		}
		else
		{
			//print(memory);
		}
		for (int j = 0; j < memoryNum; j++)
		{
			time[j]++;
		}
	}//end for
	missRate = (float)missNum / pageNum;
	printf("FIFOȱҳ������%d   ȱҳ��Ϊ:  %.2f%%\n", missNum, missRate * 100);

}
void LRU()
{
	int i;
	missNum = 0;
	curmemory = 0;
	for (i = 0; i < memoryNum; i++)
	{
		memory[i].time = 50;
	}
	printf("LRUҳ���û����:   \n");
	for (i = 0; i<pageNum; i++)
	{
		//printf("%d\t", page[i].num);
		for (int j = 0; j<memoryNum; j++)
		{
			if (memory[j].num >= 0)
				memory[j].time++;
		}
		if (Search(page[i].num, memory)<0)//����������û���ҵ���ҳ��
		{

			if (i < memoryNum){
				curmemory = i;
				memory[i].time = 0;
			}
			else
			{
				curmemory = Max(memory);//����������δʹ�õĿ�Ÿ�ֵ��curmemory
				missNum++;//ȱҳ������һ
				//printf("*");
			}
			memory[curmemory].num = page[i].num;//�����滻
			memory[curmemory].time = 0;//�滻��ʱ����Ϊ0
			//print(memory);
		}
		else
		{
			curmemory = Search(page[i].num, memory);
			memory[curmemory].time = 0;//��������ͬ���ڴ���ʱ����0
			//print(memory);
		}

	}//end for
	missRate = (float)missNum / pageNum;
	printf("LRUȱҳ������%d   ȱҳ��:  %.2f%%\n", missNum, missRate * 100);

}
void Clock(){
	int i;
	missNum = 0;
	curmemory = 0;
	printf("CLOCKҳ���û����:   \n");
	for (i = 0; i<pageNum; i++)
	{
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory) < 0)//����������û���ҵ���ҳ��
		{
			for (int j = curmemory; j < memoryNum; j++)
			{
				if (memory[j].F == 0){
					memory[j].num = page[i].num;
					memory[j].F = 1;
					curmemory++;
					if (curmemory == memoryNum){ curmemory = 0; }
					if (j == memoryNum - 1){ j = 0; }
					break;
				}
				else {
					memory[j].F = 0;
					curmemory++;
					if (curmemory = memoryNum){ curmemory = 0; }
					if (j == memoryNum - 1){ j = 0; }
				}
			}
			if (i>memoryNum){
				missNum++;//ȱҳ������һ
				//printf("*");
			}
			//print(memory);
		}
		else
		{
			curmemory = Search(page[i].num, memory);
			memory[curmemory].F = 1;
			curmemory++;
			//print(memory);
		}
	}//end for
	missRate = (float)missNum / pageNum;
	printf("CLOCKȱҳ������%d   ȱҳ��Ϊ:  %.2f%%\n", missNum, missRate * 100);

}

void improveClock() {
	int i, k;
	missNum = 0;
	curmemory = 0;
	printf("�Ľ���clockҳ���û����:   \n");
	for (i = 0; i < memoryNum; i++)
		nru[i] = 0;                                          //��¼����λ
	for (i = 0; i < pageNum; i++) {
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory) >= 0)                //���������ҵ���ҳ��
		{
			nru[Search(page[i].num, memory)] = 1;
			//print(memory);
		}
		else
		{
			if (i >= memoryNum){ missNum++; }
			for (k = 0; k < memoryNum; k++) {
				if (getS(nru[k], memory[k].M) == 4)
				{
					memory[k].num = page[i].num;
					memory[k].M = 1;
					nru[k] = 0;
					break;
				}
			}
			if (k == memoryNum)
			{
				for (k = 0; k < memoryNum; k++) {
					if (getS(nru[k], memory[k].M) == 3) {
						memory[k].num = page[i].num;
						memory[k].M = 1;
						nru[k] = 1;
						break;
					}
					else
						nru[k] = 0;
				}
			}
			if (k == memoryNum)
			{
				for (k = 0; k < memoryNum; k++) {
					if (getS(nru[k], memory[k].M) == 4) {
						memory[k].num = page[i].num;
						memory[k].M = 1;
						nru[k] = 0;
						break;
					}
				}
			}
			if (k == memoryNum)
			{
				for (k = 0; k < memoryNum; k++) {
					if (getS(nru[k], memory[k].M) == 3) {
						memory[k].num = page[i].num;
						memory[k].M = 1;
						nru[k] = 1;
						break;
					}
					else
						nru[k] = 0;
				}
			}
			//if (i >= memoryNum){ printf("*"); }
			//print(memory);
		}
	}
	missRate = (float)missNum / pageNum;
	printf("�Ľ���clockȱҳ������%d   ȱҳ��:  %.2f%%\n", missNum, missRate * 100);
}
void Start()
{
	int i;
	printf("----------------------------\n");
	printf("ҳ���û��㷨����\n");
	printf("16281001       �¿���\n");
	printf("��ѡ����Ҫʹ�õ��㷨:\n");
	printf("*****1:����û��㷨*****\n");
	printf("*****2:����û��㷨*****\n");
	printf("*****3:FIFOҳ���û�*****\n");
	printf("*****4:LRUҳ���û�*****\n");
	printf("*****5:��clockҳ���û�*****\n");
	printf("*****6:�Ľ���clockҳ���û�***\n");
	printf("*****7:��ͬһ�������в���***\n");
	printf("----------------------------\n");
	scanf_s("%d", &i);
	switch (i)
	{
	case 1: {
				creatPage();
				OPT();
	}; break;
	case 2: {
				creatPage();
				Random();
	}; break;
	case 3: {
				creatPage();
				FIFO();
	}; break;
	case 4: {
				creatPage();
				LRU();
	}; break;
	case 5: {
				creatPage();
				Clock();
	}; break;
	case 6: {
				creatPage();
				improveClock();
	}; break;
	case 7:{
			   creatPage();
			   OPT();
			   initMemory();
			   Random();
			   initMemory();
			   FIFO();
			   initMemory();
			   LRU();
			   initMemory();
			   Clock();
			   initMemory();
			   improveClock();
	}break;
	default:
	{
			   printf("�������\n");
			   Start();
	};

	}
}
void main(){
	while (1){
		Start();
	}
	system("pause");
}

