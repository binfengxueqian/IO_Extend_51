#include "queue.h"
CommandQueue commandQueue;
// 初始化队列
void initCommandQueue(CommandQueue *queue)
{
    queue->front = 0;
    queue->rear = 0;
}

// 判断队列是否为空
unsigned char isCommandQueueEmpty(CommandQueue *queue)
{
    return (queue->front == queue->rear);
}

// 判断队列是否已满
unsigned char isCommandQueueFull(CommandQueue *queue)
{
    return ((queue->rear + 1) % MAX_QUEUE_SIZE == queue->front);
}

// 入队操作
unsigned char enqueue(CommandQueue *queue, CommandStruct command)
{
    if (isCommandQueueFull(queue))
    {
        // 队列已满，无法入队
        return 0;
    }

    // 将数据存入队尾
    queue->command[queue->rear] = command;
    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE; // 队尾指针循环移动

    return 1;
}

// 出队操作
unsigned char dequeue(CommandQueue *queue, CommandStruct *command)
{
    if (isCommandQueueEmpty(queue))
    {
        // 队列为空，无法出队
        return 0;
    }

    // 获取队首数据
    *command = queue->command[queue->front];
    queue->front = (queue->front + 1) % MAX_QUEUE_SIZE; // 队首指针循环移动

    return 1;
}
