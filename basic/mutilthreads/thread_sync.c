#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 16

typedef struct ringbuf_s ringbuf_t;
struct ringbuf_s{
    int buffer[BUFFER_SIZE]; /* 实际数据存放的数组 */
    int readpos;    /* 读指针 */
    int writepos;   /* 写指针 */
    pthread_mutex_t mutex;    /* 互斥体lock 用于对缓冲区的互斥操作 */
    pthread_cond_t notempty; /* 缓冲区非空的条件变量 */
    pthread_cond_t notfull;  /* 缓冲区未满的条件变量 */
};

/* 初始化缓冲区结构 */
void init_ringbuf(ringbuf_t *buf)
{
    buf->readpos = 0;
    buf->writepos = 0;
    pthread_mutex_init(&buf->mutex, NULL);
    pthread_cond_init(&buf->notempty, NULL);
    pthread_cond_init(&buf->notfull, NULL);
}

/* 将数据放入缓冲区 */
void put(ringbuf_t *buf, int data)
{
    pthread_mutex_lock(&buf->mutex);

    /* 等待缓冲区未满*/
    if ((buf->writepos + 1) % BUFFER_SIZE == buf->readpos)  // 判断缓冲区已满
    {
        pthread_cond_wait(&buf->notfull, &buf->mutex);   // 阻塞
    }

    /* 写数据,并移动指针 */
    buf->buffer[buf->writepos] = data;
    buf->writepos++;
    // writepos已满，从0开始
    if (buf->writepos == BUFFER_SIZE)
        buf->writepos = 0;

    /* 设置缓冲区非空的条件变量 */
    pthread_cond_signal(&buf->notempty);

    pthread_mutex_unlock(&buf->mutex);
}

/* 从缓冲区中取出数据 */
int get(ringbuf_t *buf)
{
    int data;

    pthread_mutex_lock(&buf->mutex);

    /* 等待缓冲区非空 */
    if (buf->writepos == buf->readpos)
    {
        pthread_cond_wait(&buf->notempty, &buf->mutex);
    }

    /* 读数据,移动读指针 */
    data = buf->buffer[buf->readpos];
    buf->readpos++;

    if (buf->readpos == BUFFER_SIZE)
        buf->readpos = 0;

    /* 设置缓冲区未满的条件变量 */
    pthread_cond_signal(&buf->notfull);
    pthread_mutex_unlock(&buf->mutex);

    return data;
}

/* 测试:生产者线程将1 到10000 的整数送入缓冲区,消费者线程从缓冲区中获取整数,两者都打印信息*/
#define OVER (-1)

ringbuf_t g_buffer;

void *producer(void *data)
{
    int n;

    for (n = 0; n < 100; n++)
    {
        printf("%d —-> in\n", n);
        put(&g_buffer, n);
    }

    put(&g_buffer, OVER);

    return NULL;
}

void *consumer(void *data)
{
    int d;
    while (1)
    {
        d = get(&g_buffer);
        sleep(1);

        if (d == OVER)
            break;
        printf("out <-— %d\n", d);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tp, tc;
    void *retval;

    init_ringbuf(&g_buffer);

    /* 创建生产者和消费者线程*/
    pthread_create(&tp, NULL, producer, 0);
    pthread_create(&tc, NULL, consumer, 0);

    /* 等待两个线程结束*/
    pthread_join(tp, &retval);
    pthread_join(tc, &retval);

    return 0;
}