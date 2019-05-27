// 页面置换.cpp : 定义控制台应用程序的入口点。
//
// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//
#include"stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct item
{
	int num;         //页号
	int time;        //等待时间
	int M;         //修改位
	int F;         //访问位
}Pro;
int curmemory;         //调入主存中的页面个数
int missNum;           //缺页次数
float missRate;        //缺页率
int pageNum;           //系统分配给作业的主存中的页面数
int memoryNum;         //可用内存页面数
int nru[200] = { 0 };

Pro  page[100000];             //作业页面集
Pro  memory[20];          //内存页面集

void print(Pro *memory1)//打印当前的页面
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
	for (i = 0; i<memoryNum; i++)        //初始化内存中页面
	{
		memory[i].num = -1;                //页面为空用-1表示
		memory[i].time = -1;
		memory[i].M = 0;                   //修改位初始化为0
		memory[i].F = 0;					//访问位初始化
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
int Max(Pro *memory1)//寻找最近最长未使用的页面
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
	printf("输入系统分配给作业的主存中的页面数:");
	scanf_s("%d", &pageNum);
	printf("输入内存页面数:");
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

int optimal(int num, int tag, Pro *memory1, Pro *page1)//最佳置换算法
{
	int k, j, min[20];
	for (k = 0; k<memoryNum; k++)//k<可用内存页面数
		min[k] = 500;
	for (k = 0; k<memoryNum; k++)
	{
		j = tag;//记录经过多少次后被访问
		do {

			j++;
			if (j>pageNum)
				break;
		} while (page1[j].num != memory1[k].num);//页面再次被访问时循环结束
		if (j<500)
		{

			min[k] = j;//获得页面下一次被访问的次数
		}
	}
	int max = 0;
	for (int t = 1; t<memoryNum; t++)
	{
		if (min[t]>min[max])//判断哪一个页面是最长未来时间内不再被访问的
			max = t;
	}
	return max;
}
void OPT()
{
	int i;
	missNum = 0;
	curmemory = 0;
	printf("最佳置换算法置换情况:   \n");
	for (i = 0; i<pageNum; i++)
	{
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory)<0)//若在主存中没有找到该页面
		{
			if (i<memoryNum)//i<可用内存页面数
				curmemory = i;
			else
			{
				curmemory = optimal(page[i].num, i, memory, page);
				missNum++;//缺页次数+1
				//printf("*");
			}
			memory[curmemory].num = page[i].num;//把调换出来的当前页放进内存中
			//print(memory);
		}
		else
		{
			/*print(memory);*/
		}
	}//end for
	missRate = (float)missNum / pageNum;
	printf("最佳置换算法的缺页次数为：%d   缺页率为:  %.2f%%\n", missNum, missRate * 100);

}
void Random(){
	int i;
	missNum = 0;
	curmemory = 0;
	printf("随机置换算法置换情况:   \n");
	for (i = 0; i < pageNum; i++){
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory) < 0)//若在主存中没有找到该页面
		{
			if (i<memoryNum)//i<可用内存页面数
				curmemory = i;
			else
			{
				curmemory = rand() % memoryNum;
				missNum++;//缺页次数+1
				//printf("*");
			}
			memory[curmemory].num = page[i].num;//把调换出来的当前页放进内存中
			//print(memory);
		}
		else
		{
			//print(memory);
		}
	}
	missRate = (float)missNum / pageNum;
	printf("随机置换算法的缺页次数为：%d   缺页率为:  %.2f%%\n", missNum, missRate * 100);

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
	printf("FIFO页面置换情况:   \n");
	for (i = 0; i<pageNum; i++)
	{
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory) < 0)//若在主存中没有找到该页面
		{
			if (i<memoryNum)//i<可用内存页面数
				curmemory = i;
			else
			{
				max = 0;
				for (int j = 1; j < memoryNum; j++)
				{
					if (time[j]>time[max])max = j;
				}
				curmemory = max;
				missNum++;//缺页次数+1
				//printf("*");
				time[curmemory] = 0;
			}
			memory[curmemory].num = page[i].num;//把调换出来的当前页放进内存中
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
	printf("FIFO缺页次数：%d   缺页率为:  %.2f%%\n", missNum, missRate * 100);

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
	printf("LRU页面置换情况:   \n");
	for (i = 0; i<pageNum; i++)
	{
		//printf("%d\t", page[i].num);
		for (int j = 0; j<memoryNum; j++)
		{
			if (memory[j].num >= 0)
				memory[j].time++;
		}
		if (Search(page[i].num, memory)<0)//若在主存中没有找到该页面
		{

			if (i < memoryNum){
				curmemory = i;
				memory[i].time = 0;
			}
			else
			{
				curmemory = Max(memory);//返回最近最久未使用的块号赋值给curmemory
				missNum++;//缺页次数加一
				//printf("*");
			}
			memory[curmemory].num = page[i].num;//进行替换
			memory[curmemory].time = 0;//替换后时间置为0
			//print(memory);
		}
		else
		{
			curmemory = Search(page[i].num, memory);
			memory[curmemory].time = 0;//把与它相同的内存块的时间置0
			//print(memory);
		}

	}//end for
	missRate = (float)missNum / pageNum;
	printf("LRU缺页次数：%d   缺页率:  %.2f%%\n", missNum, missRate * 100);

}
void Clock(){
	int i;
	missNum = 0;
	curmemory = 0;
	printf("CLOCK页面置换情况:   \n");
	for (i = 0; i<pageNum; i++)
	{
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory) < 0)//若在主存中没有找到该页面
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
				missNum++;//缺页次数加一
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
	printf("CLOCK缺页次数：%d   缺页率为:  %.2f%%\n", missNum, missRate * 100);

}

void improveClock() {
	int i, k;
	missNum = 0;
	curmemory = 0;
	printf("改进型clock页面置换情况:   \n");
	for (i = 0; i < memoryNum; i++)
		nru[i] = 0;                                          //记录访问位
	for (i = 0; i < pageNum; i++) {
		//printf("%d\t", page[i].num);
		if (Search(page[i].num, memory) >= 0)                //若在主存找到该页面
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
	printf("改进型clock缺页次数：%d   缺页率:  %.2f%%\n", missNum, missRate * 100);
}
void Start()
{
	int i;
	printf("----------------------------\n");
	printf("页面置换算法程序\n");
	printf("16281001       陈俊良\n");
	printf("请选择想要使用的算法:\n");
	printf("*****1:最佳置换算法*****\n");
	printf("*****2:随机置换算法*****\n");
	printf("*****3:FIFO页面置换*****\n");
	printf("*****4:LRU页面置换*****\n");
	printf("*****5:简单clock页面置换*****\n");
	printf("*****6:改进型clock页面置换***\n");
	printf("*****7:用同一样例进行测试***\n");
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
			   printf("输入错误！\n");
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

