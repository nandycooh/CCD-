#ifndef __MYQUEUE_H
#define __MYQUEUE_H

#include<stdbool.h>

#define MAXSIZE 4
#define MAXSIZES 501
#define MAXSIZE_A 301
//#define MAXSIZES 201
//#define MAXSIZE_A 101

//������нṹ��
typedef struct 
{
    float data[MAXSIZE];
    int front;
    int rear;
}Que_typedef;

typedef struct 
{
    float data[MAXSIZES];
    int front;
    int rear;
}Que_typedef_small;

typedef struct 
{
    float data0[MAXSIZE_A];
    float data1[MAXSIZE_A];
    float data2[MAXSIZE_A];
    float data[MAXSIZE_A];
    int front;
    int rear;
}Que_typedef_accel;




//�ж϶����Ƿ�Ϊ��
bool Empty(Que_typedef *q);

//�ж϶����Ƿ�����
bool full(Que_typedef *q);

//��ʼ������
void init_que(Que_typedef *q);
void init_que_small(Que_typedef_small *q);
void init_que_accel(Que_typedef_accel *q);




#endif
