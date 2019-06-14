// wenjian.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define FILEBLOCK 1
#define DATABLOCK 4
#define FILEDESSIZE 100
#define L 1129
#define B 32
#define KEEPSIZE  105

typedef struct
{
    char name[10];
    short int length;
    short int allo[10];
} filedes;

typedef struct
{
    char Buffer[B];
    char *p;
    int id;
    int index;
    filedes dsc;
} OpenFileTable;

bool read_block(int i,char *p);
bool write_block(int i,char *p);
void InitFS();
bool create(char *filename);
bool destroy(char *filename);
bool open(char *filename);
bool close(char *index);
bool read(char *index, char *mem_area, int count);
bool write(char *index, char *mem_area, int count);
bool directory();
int getfreedatablock();
int getfileheadblock();
void setfileheadblock();
bool addfile(int fileIndex);
void savedata();
void loaddata();

char ldisk[L][B];
char map[8] = {128,64,32,16,8,4,2,1};
char temp_block[B];
int noofblock;
OpenFileTable openFileTable;

bool read_block(int i,char *p)
{
    memcpy(p,ldisk[i],B);
    return true;
}

bool write_block(int i,char *p)
{
    memcpy(ldisk[i],p,B);
    return true;
}

void InitFS()
{
    char filename[] = "catalog";
    char *p = filename;
    int length=0;
    int index  = getfileheadblock();
    filedes fd;
    memset(ldisk,0,L*B);
    memcpy(&fd.name,filename,length+1);
    fd.length=-1;
    for (int i=0; i<10; i++)
    {
        fd.allo[i]=-1;
    }
    write_block(FILEBLOCK+DATABLOCK+index,(char*)&fd);
    noofblock = 0;
    setfileheadblock();
    return ;
}

bool create(char *filename)
{
    char *p = filename;
    int length=0;
    if((length = strlen(filename))>10)
    {
        printf("�ļ���̫����\n");
        return false;
    }
    int index  = getfileheadblock();
    setfileheadblock();
    p=filename;
    filedes fd;
    memcpy(&fd.name,filename,length+1);
    fd.length = 0;
    for (int i=0; i<10; i++)
    {
        fd.allo[i]=-1;
    }
    fd.allo[0] = getfreedatablock();
    setfileheadblock();
    memset(temp_block,-1,B);
    write_block(fd.allo[0],temp_block);
    write_block(FILEBLOCK+DATABLOCK+index,(char*)&fd);
    addfile(index);
    return true;
}

bool destroy(char *filename)
{
    char temp_block[B];
    filedes fd;
    read_block(DATABLOCK+FILEBLOCK,(char*)&fd);
    if(fd.allo[0]==-1)
    {
        printf("�ļ�������!\n");
        return false;
    }
    for (int i=0; i<10; i++)
    {
        if(fd.allo[i]==-1)continue;
        read_block(fd.allo[i],temp_block);
        for (int j=0; j<B; j++)
        {
            if(temp_block[j]!=-1)
            {
                filedes tfd;
                read_block(DATABLOCK+FILEBLOCK+temp_block[j],(char*)&tfd);
                if(memcmp(filename,tfd.name,strlen(filename))==0)
                {
                    int fileIndex = temp_block[j];
                    temp_block[j]=-1;
                    write_block(fd.allo[i],temp_block);
                    read_block(0,temp_block);
                    int a = fileIndex/8;
                    int b = fileIndex%8;
                    temp_block[a] = temp_block[a]&(~map[b]);
                    write_block(0,temp_block);
                }
            }
        }
    }
    return true;
}

bool open(char *filename)
{
    filedes fd;
    read_block(DATABLOCK+FILEBLOCK,(char*)&fd);
    for (int i=0; i<10; i++)
    {
        if(fd.allo[i]==-1)
        {
            printf("�ļ�������!\n");
            return false;
        }
        read_block(fd.allo[i],temp_block);
        for (int j=0; j<B; j++)
        {
            if(temp_block[j]!=-1)
            {
                filedes temp_fd;
                read_block(DATABLOCK+FILEBLOCK+temp_block[j],(char*)&temp_fd);
                if(memcmp(filename,temp_fd.name,strlen(filename))==0)
                {
                    openFileTable.id = temp_block[j];
                    openFileTable.index = 0;
                    if(temp_fd.allo[0]==-1)
                    {
                        openFileTable.p==NULL;
                    }
                    read_block(temp_fd.allo[0],openFileTable.Buffer);
                    openFileTable.p = openFileTable.Buffer;
                    openFileTable.dsc = temp_fd;
                    return true;
                }

            }
        }
    }
    return false;
}

bool close(char *index)
{
    if(openFileTable.id==-1)
    {
        printf("û�д򿪵��ļ�\n");
        return false;
    }
    openFileTable.id=-1;
    return true;
}

bool read(char *index, char *mem_area, int count)
{
    int left=B-(openFileTable.p-openFileTable.Buffer);
    while (count)
    {
        if(count-left>0) //��ǰ��������������������ȡ
        {
            count-=left;
            if(openFileTable.p==NULL)
            {
                printf("Խ�����\n");
                return false;
            }
            memcpy(mem_area,openFileTable.p,left);
            openFileTable.index++;
            if(openFileTable.dsc.allo[openFileTable.index]==-1)
            {
                openFileTable.p=NULL;
            }
            read_block(openFileTable.dsc.allo[openFileTable.index],openFileTable.p);
            left = B;
        }
        else
        {
            if(openFileTable.p==NULL)
            {
                printf("Խ�����\n");
                return false;
            }
            memcpy(mem_area,openFileTable.p,count);
            write_block(openFileTable.dsc.allo[openFileTable.index],openFileTable.Buffer);
            openFileTable.p+=left;
            count=0;
            openFileTable.p+=count;
        }
    }

    return true;
}


bool write(char *index, char *mem_area, int count)
{

    int left=B-(openFileTable.p-openFileTable.Buffer);
    while (count)
    {
        if(count-left>0) //��ǰ��������������������ȡ
        {
            count-=left;
            if(openFileTable.p==NULL)
            {
                printf("���ʳ�����Χ\n");
                return false;
            }
            memcpy(openFileTable.Buffer,mem_area,left);
            write_block(openFileTable.dsc.allo[openFileTable.index],openFileTable.Buffer);
            openFileTable.index++;
            if(openFileTable.dsc.allo[openFileTable.index]==-1)
            {
                openFileTable.p=NULL;
            }
            openFileTable.p = openFileTable.Buffer;

            left = B;
        }
        else
        {
            if(openFileTable.p==NULL)
            {
                printf("���ʳ�����Χ\n");
                return false;
            }
            memcpy(openFileTable.p,mem_area,count);
            write_block(openFileTable.dsc.allo[openFileTable.index],openFileTable.Buffer);

            openFileTable.p+=count;
            openFileTable.dsc.length+=count;
            write_block(DATABLOCK+FILEBLOCK+openFileTable.id,(char*)&openFileTable.dsc);
            return true;
        }
    }

    return false;
}

bool directory()
{
    filedes fd;
    read_block(FILEBLOCK+DATABLOCK,(char*)&fd);
    printf("�ļ�����   �ļ���С\n");
    for (int i=0; i<10; i++)
    {
        if(fd.allo[i]!=-1)
        {
            read_block(fd.allo[i],temp_block);
            for (int j=0; j<B; j++)
            {
                if(temp_block[j]!=-1)
                {
                    filedes temp[B];
                    read_block(FILEBLOCK+DATABLOCK+temp_block[j],(char*)&temp);
                    printf("%s\t\t",temp->name);
                    printf("%d\n",temp->length);
                }
            }
        }

    }
    return true;
}

int getfreedatablock()
{
    int bIndex = FILEBLOCK;
    while (1)
    {
        read_block(bIndex++,temp_block);
        for (int i=0; i<B; i++)
        {
            if(temp_block[i]!=0xff)
            {
                for (int k=0; k<8; k++)
                {
                    if(!(map[k]&temp_block[i]))
                    {
                        temp_block[i] = map[k]|temp_block[i];
                        noofblock = bIndex-1;
                        return KEEPSIZE+(bIndex-2)*B*8 + i*8 +k;
                    }
                }
            }
        }
    }
    return 0;
}

int getfileheadblock()
{
    read_block(0,temp_block);
    char *p = temp_block;
    for(int i=0; i<B*FILEBLOCK; i++)
    {
        for (int k=0; k<8; k++)
        {
            int tt = *p&map[k];
            if(!tt)
            {
                *p=*p^map[k];
                noofblock = 0;
                return i*8+k;
            }
        }
        p++;
    }
    return -1;
}
void setfileheadblock()
{
    write_block(noofblock,temp_block);
}

bool addfile(int fileIndex)
{
    filedes fp;
    read_block(DATABLOCK+FILEBLOCK,(char *)&fp);
    int dataIndex=0;

    while (dataIndex<10)
    {
        if(fp.allo[dataIndex]==-1)
        {
            fp.allo[0] = getfreedatablock();
            setfileheadblock();
            temp_block[0] = fileIndex;
            for (int i=1; i<B; i++)
            {
                temp_block[i]=-1;
            }
            write_block(fp.allo[dataIndex],temp_block);
            write_block(DATABLOCK+FILEBLOCK,(char *)&fp);
            return true;
        }
        read_block(fp.allo[dataIndex++],temp_block);
        for (int i=0; i<B; i++)
        {
            if(temp_block[i]==-1)
            {
                temp_block[i] = fileIndex;
                write_block(fp.allo[dataIndex-1],temp_block);
                return true;
            }
        }
    }
    return false;
}

void savedata()
{

    FILE *fp = fopen("data.txt","w");
    if(fp==NULL)
    {
        printf("�޷��򿪴洢�ļ�\n");
        return;
    }
    fwrite(&ldisk[0][0],1,L*B,fp);
    fclose(fp);
}

void loaddata()
{
    FILE *fp = fopen("data.txt","r");
    if(fp==NULL)
    {
        printf("�޷��򿪴洢�ļ�\n");
        return;
    }
    fread(&ldisk[0][0],1,L*B,fp);
    fclose(fp);
}

int main()
{
    InitFS();
    loaddata();
    filedes t;
    while(1)
    {
        printf("|*���˵���                                            *|\n");
        printf("|*1 �����ļ�                                          *|\n");
        printf("|*2 ɾ���ļ�                                          *|\n");
        printf("|*3 ���ļ�                                          *|\n");
        printf("|*4 �鿴Ŀ¼                                          *|\n");
        printf("|*5 ��ʽ��                                            *|\n");
        printf("|*0 �˳�                                              *|\n");
        printf("��ѡ�������");
        char ch;
        ch = getchar();
        getchar();
        switch(ch)
        {
        case '0':
            savedata();
            return 0;
        case '1':
            printf("�������ļ���:\n");
            char filename1[10];
            scanf("%s",filename1);
            if(create(filename1))
            {
                printf("�����ɹ�!\n");
            }
            else
                printf("����ʧ��!\n");
            getchar();
            break;
        case '2':
            printf("������Ҫɾ�����ļ���:");
            char filename[10];
            scanf("%s",filename);
            getchar();
            printf("�ɹ�ɾ��!\n");
            destroy(filename);

            break;
        case '3':
            printf("�������ļ���: ");
            scanf("%s",filename);
            getchar();
            if(!open(filename))
            {
                break;
            }
            printf("1 ��ȡ�ļ�\n");
            printf("2 д���ļ�\n");
            printf("��ѡ��");
            ch = getchar();
            switch(ch)
            {
            case '1':
                getchar();
                char temp[B];
                read(NULL,temp,B);
                printf("%s\n",temp);
                close(filename);
                break;
            case '2':
                getchar();
                char str[100];
                printf("������д�����ݣ�\n");
                scanf("%s",str);
                write(NULL,str,strlen(str)+1);
                close(filename);
                getchar();
                break;
            default:
                break;
            }
        case '4':
            directory();
            printf("\n");
            break;
        case '5':
            InitFS();
            printf("��ʼ���ɹ���\n");
            break;
        }
    }
    return 0;
}

