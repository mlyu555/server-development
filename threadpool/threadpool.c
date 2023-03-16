#include <pthread.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>


#define LL_ADD(item, list) do { 	\
	item->prev = NULL;				\
	item->next = list;				\
	list = item;					\
} while(0)

#define LL_REMOVE(item, list) do {						\
	if (item->prev != NULL) item->prev->next = item->next;	\
	if (item->next != NULL) item->next->prev = item->prev;	\
	if (list == item) list = item->next;					\
	item->prev = item->next = NULL;							\
} while(0)

typedef struct _ThreadPool ThreadPool;

struct Task {
    void (*callback)(void *arg);
    void *userdata;

    struct Task *prev;
    struct Task *next;
};

struct Worker {
    pthread_t pid;          // 关联线程
    int terminal;
    ThreadPool *pool;

    struct Worker *prev;
    struct Worker *next;
};

struct _ThreadPool {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;

    struct Task *tasks;
    struct Worker *workers;
};

static void *worker_thread(void *arg) {
    struct Worker *worker = (struct Worker *)arg;

    while (1) {


        pthread_mutex_lock(&worker->pool->mutex);   // 为条件变量pthread_cond_wait加锁

        while (worker->pool->tasks == NULL) {
            if (worker->terminal) break;

            pthread_cond_wait(&worker->pool->cond, &worker->pool->mutex);
        }

        if (worker->terminal) {
            pthread_mutex_unlock(&worker->pool->mutex);
            break;
        }

        // 获取任务
        struct Task *task = worker->pool->tasks;
        if (task != NULL) {
            LL_REMOVE(task, worker->pool->tasks);
        }

        pthread_mutex_unlock(&worker->pool->mutex);

        if (task == NULL) continue;

        // 执行回调函数
        task->callback(task);
    }

    free(worker);
    pthread_exit(NULL);
}

int threadpool_init(ThreadPool *pool, int number) {
    if (number < 0) {
        perror("thread number set");
        return EXIT_FAILURE;
    }

    // pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    // if (pool == NULL) {
    //     perror("malloc thread pool");
    //     return EXIT_FAILURE;
    // }
    memset(pool, 0, sizeof(ThreadPool));

    // 初始化
    // 方式一
    pthread_mutex_t blank_mutex = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&pool->mutex, &blank_mutex, sizeof(pool->mutex));
    pthread_cond_t blank_cond = PTHREAD_COND_INITIALIZER;
    memcpy(&pool->cond, &blank_cond, sizeof(pool->cond));

    // 方式二 等价方式一
    // pthread_mutex_init(&pool->mutex, NULL);
    // pthread_cond_init(&pool->cond, NULL);

    int i = 0;
    for ( ; i < number; i++) {
        struct Worker *worker = (struct Worker *)malloc(sizeof(struct Worker));
        if (worker == NULL) {
            perror("malloc worker");
            return EXIT_FAILURE;
        }
        memset(worker, 0, sizeof(struct Worker));
        worker->pool = pool;

        int ret = pthread_create(&worker->pid, NULL, worker_thread, (void *)worker);
        if (ret < 0) {
            perror("pthread_create");
            free(worker);
            return EXIT_FAILURE;
        }


        // todo worker->terminal 未初始化

        LL_ADD(worker, pool->workers);
    }

    return 0;
}


int threadpool_destory(ThreadPool *pool) {
    // free(pool);

    struct Worker *worker = NULL;

    // 为什么此处是置状态而非直接退出线程并释放worker
    for (worker == pool->workers; worker != NULL; worker = worker->next) {
        worker->terminal = 1;
    }

    pthread_mutex_lock(&pool->mutex);
    pool->workers = NULL;
    pool->tasks = NULL;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);

    return 0;
}

int threadpool_push_task(ThreadPool *pool, struct Task *task) {
    // 1. 添加到任务队列
    // 2. 通知（唤醒）工作队列

    pthread_mutex_lock(&pool->mutex);
    LL_ADD(task, pool->tasks);                  // 思考：是否可以放在加锁前？
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);

    return 0;
}


void counter(void *arg) {
    int index;
    struct Task *task = (struct Task *)arg;
    index = *(int *)task->userdata;

    printf("index: %d, pid: %lu\n", index, pthread_self());

    free(task->userdata);
    free(task);
}


int main(int argc, char *argv[]) {


    ThreadPool pool;
    threadpool_init(&pool, 10);

    int i = 0;
    for (i = 0; i < 1000; i++) {
        struct Task *task = (struct Task *)malloc(sizeof(struct Task));
        if (task == NULL) {
            perror("malloc");
            return EXIT_FAILURE;
        }

        task->callback = counter;
        task->userdata = malloc(sizeof(int));
        *(int *)task->userdata = i;

        threadpool_push_task(&pool, task);
    }


    getchar();
    printf("\n");

    return 0;
}