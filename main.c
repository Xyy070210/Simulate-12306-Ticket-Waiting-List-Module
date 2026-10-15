#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE_SIZE 100

//候补订单结构体
typedef struct Order {
    char orderId[20];     //订单号
    char name[20];        //姓名
    char idCard[20];      //身份证号
    int ticketNum;        //需要票数
}Order;

//队列结构
typedef struct Queue {
    Order data[MAX_QUEUE_SIZE];
    int front;  //队头
    int rear;   //队尾
}Queue;

//初始化队列
void InitQueue(Queue *q)
{
    q->front = 0;
    q->rear = 0;
}

//判断队列是否满
int IsFull(Queue *q)
{
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

//判断队列是否空
int IsEmpty(Queue *q)
{
    return q->front == q->rear;
}

//入队：新增候补订单，加到队尾
int EnQueue(Queue *q, Order ord)
{
    if(IsFull(q))
    {
        printf("候补队列已满，无法新增订单！\n");
        return 0;
    }
    q->data[q->rear] = ord;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    printf("候补订单提交成功，进入排队！\n");
    return 1;
}

//出队：取出队头订单
int DeQueue(Queue *q, Order *ord)
{
    if(IsEmpty(q))
    {
        return 0;
    }
    *ord = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return 1;
}

//发放余票，参数k：本次释放票数
void DistributeTicket(Queue *q, int k)
{
    if(IsEmpty(q))
    {
        printf("当前无候补排队订单！\n");
        return;
    }
    printf("\n-----开始分配余票，本次可分配票数：%d -----\n",k);
    while(k>0 && !IsEmpty(q))
    {
        Order ord;
        DeQueue(q, &ord);
        if(ord.ticketNum <= k)
        {
            //可以满足这个订单，出票成功
            printf("出票成功！订单号：%s，姓名：%s，身份证：%s，申请%d张票\n",
                   ord.orderId, ord.name, ord.idCard, ord.ticketNum);
            k -= ord.ticketNum;
        }
        else
        {
            //余票不够该订单，重新放回队列头部（基础版可简化：跳过，放回队首）
            printf("剩余票数不足，订单【%s】暂无法出票，继续排队\n",ord.orderId);
            //简易处理：把这个订单塞回队列（环形队列简易回塞）
            q->front = (q->front -1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
            break;
        }
    }
    if(k>0)
        printf("本次分配结束，剩余未分配票数：%d\n",k);
    else
        printf("本次余票全部发放完毕\n");
}

//打印菜单
void ShowMenu()
{
    printf("\n=========12306候补车票模拟系统=========\n");
    printf("1.新增候补订单（入队）\n");
    printf("2.发放余票，处理候补队列\n");
    printf("3.查看当前排队人数\n");
    printf("4.退出程序\n");
    printf("请输入你的选择：");
}

int main()
{
    Queue q;
    InitQueue(&q);
    int choice;
    while(1)
    {
        ShowMenu();
        scanf("%d",&choice);
        getchar();
        if(choice == 1)
        {
            Order newOrd;
            printf("请输入订单号：");
            scanf("%s",newOrd.orderId);
            getchar();
            printf("请输入旅客姓名：");
            scanf("%s",newOrd.name);
            getchar();
            printf("请输入身份证号：");
            scanf("%s",newOrd.idCard);
            getchar();
            printf("需要车票数量：");
            scanf("%d",&newOrd.ticketNum);
            EnQueue(&q, newOrd);
        }
        else if(choice ==2)
        {
            int k;
            printf("输入本次释放余票数量k=");
            scanf("%d",&k);
            DistributeTicket(&q,k);
        }
        else if(choice ==3)
        {
            int cnt = (q.rear - q.front + MAX_QUEUE_SIZE)%MAX_QUEUE_SIZE;
            printf("当前候补排队人数：%d\n",cnt);
        }
        else if(choice ==4)
        {
            printf("程序退出\n");
            break;
        }
        else
        {
            printf("输入错误，请重新选择\n");
        }
    }
    return 0;
}

