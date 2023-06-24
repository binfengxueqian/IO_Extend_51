#include "queue.h"
CommandQueue commandQueue;
// ��ʼ������
void initCommandQueue(CommandQueue *queue)
{
    queue->front = 0;
    queue->rear = 0;
}

// �ж϶����Ƿ�Ϊ��
unsigned char isCommandQueueEmpty(CommandQueue *queue)
{
    return (queue->front == queue->rear);
}

// �ж϶����Ƿ�����
unsigned char isCommandQueueFull(CommandQueue *queue)
{
    return ((queue->rear + 1) % MAX_QUEUE_SIZE == queue->front);
}

// ��Ӳ���
unsigned char enqueue(CommandQueue *queue, CommandStruct command)
{
    if (isCommandQueueFull(queue))
    {
        // �����������޷����
        return 0;
    }

    // �����ݴ����β
    queue->command[queue->rear] = command;
    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE; // ��βָ��ѭ���ƶ�

    return 1;
}

// ���Ӳ���
unsigned char dequeue(CommandQueue *queue, CommandStruct *command)
{
    if (isCommandQueueEmpty(queue))
    {
        // ����Ϊ�գ��޷�����
        return 0;
    }

    // ��ȡ��������
    *command = queue->command[queue->front];
    queue->front = (queue->front + 1) % MAX_QUEUE_SIZE; // ����ָ��ѭ���ƶ�

    return 1;
}
