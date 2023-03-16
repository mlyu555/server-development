#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


#define PAGESIZE 4096   // 4k

typedef struct mp_block_s mp_block_t;
typedef struct mp_large_s mp_large_t;
typedef struct mp_mempool_s mp_mempool_t;

#define MP_ALIGNMENT       		32
#define MP_PAGE_SIZE			4096
#define MP_MAX_ALLOC_FROM_POOL	(MP_PAGE_SIZE-1)

#define mp_align(n, alignment) (((n)+(alignment-1)) & ~(alignment-1))
#define mp_align_ptr(p, alignment) (void *)((((size_t)p)+(alignment-1)) & ~(alignment-1))

struct mp_block_s {
    unsigned char *last;
    unsigned char *end;
    // size_t failed;

    struct mp_block_s *next;
};


struct mp_large_s {
    struct mp_large_s *next;
    void *alloc;
};


struct mp_mempool_s
{
    size_t max;

    struct mp_block_s *current;
    struct mp_large_s *large;

    struct mp_block_s *head;
	// struct mp_node_s head[0];
};

struct mp_mempool_s *mp_create(size_t size);
void *mp_alloc(struct mp_mempool_s *pool, size_t size);
// void *mp_nalloc(struct mp_mempool_s *pool, size_t size);
// void *mp_calloc(struct mp_mempool_s *pool, size_t size);
void mp_free(struct mp_mempool_s *pool, void *p);
void mp_destroy(struct mp_mempool_s *pool);
static void *_mp_large_alloc(struct mp_mempool_s *pool, size_t size);
static void *_mp_block_alloc(struct mp_mempool_s *pool, size_t size);

static void *_mp_large_alloc(struct mp_mempool_s *pool, size_t size) {
    unsigned char *res;
    res = (unsigned char *)malloc(size);
    if (res == NULL) {
        perror("malloc: alloc large failed");
        return NULL;
    }

    struct mp_large_s *large = (struct mp_large_s *)mp_alloc(pool, sizeof(struct mp_large_s));
    large->next = pool->large;
    large->alloc = res;
    pool->large = large;

    return res;
}

static void *_mp_block_alloc(struct mp_mempool_s *pool, size_t size) {
    unsigned char *res;

    struct mp_block_s *block;
    block = (struct mp_block_s *)malloc(pool->max + sizeof(struct mp_block_s));
    if (res == NULL) {
        perror("malloc: alloc block failed");
        return NULL;
    }

    block->end = (unsigned char *)block + pool->max + sizeof(struct mp_block_s);
    block->last = (unsigned char *)block + sizeof(struct mp_block_s) + size;
    block->next = NULL;

    struct mp_block_s *tmp;
    for (tmp = pool->current; tmp->next != NULL; tmp = tmp->next) {
        continue;
    }
    tmp->next = block;

    return res;
}

struct mp_mempool_s *mp_create(size_t size) {
    struct mp_mempool_s *pool = NULL;

    pool = (struct mp_mempool_s *)malloc(size + sizeof(struct mp_mempool_s) + sizeof(struct mp_block_s));
    // int ret = posix_memalign((void **)&pool, MP_ALIGNMENT, size + sizeof(struct mp_mempool_s) + sizeof(struct mp_block_s));
    if (pool == NULL) {
        perror("mp_create failed, malloc");
        return NULL;
    }

    pool->max = (size < MP_MAX_ALLOC_FROM_POOL) ? size : MP_MAX_ALLOC_FROM_POOL;
    pool->head = (struct mp_block_s *)((unsigned char *)pool + sizeof(struct mp_mempool_s)); // 等价 pool->head = pool + 1;
    pool->current = pool->head;
    pool->large = NULL;

    pool->head->last = (unsigned char *)pool->head + sizeof(struct mp_block_s);
    pool->head->end = pool->head->last + size;
    pool->head->next = NULL;

    printf("pool start at: \t%p\n", pool);
    printf("pool->max: \t\t%d\n", pool->max);
    printf("pool->large: \t\t%p\n", pool->large);
    printf("pool->current: \t\t%p\n", pool->current);
    printf("pool->head: \t\t%p\n", pool->head);
    printf("pool->head->last: \t%p\n", pool->head->last);
    printf("pool->head->end: \t%p\n", pool->head->end);
    printf("sizeof(struct mp_block_s): \t%d\n", sizeof(struct mp_block_s));

    return pool;
}

void *mp_alloc(struct mp_mempool_s *pool, size_t size) {
    unsigned char *res;
    if (size > pool->max) {
        return _mp_large_alloc(pool, size);
    } else {
        struct mp_block_s *block = NULL;
        block = pool->current;
        if ((block->end - block->last) > size)
        {
            res = block->last;
            block->last = block->last + size;
            return res;
        }
        else
        {
            return _mp_block_alloc(pool, size);
        }
    }
}

void mp_destroy(struct mp_mempool_s *pool) {
    struct mp_large_s *large;
    struct mp_block_s *block;

    for (large = pool->large; large != NULL; large = large->next) {
        if (large->alloc != NULL) {
            free(large->alloc);
        }
    }

    // 从第2个block节点删除
    for (block = pool->head->next; block != NULL; block = block->next) {
        free(block);
    }

    free(pool);
}

void mp_free(struct mp_mempool_s *pool, void *p) {
    struct mp_large_s *l;
    for (l = pool->large; l; l = l->next)
    {
        if (p == l->alloc)
        {
            free(l->alloc);
            l->alloc = NULL;
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    // size_t size = 1 << 12;
    size_t size = 1 << 20;

    struct mp_mempool_s *pool = mp_create(size);
    printf("pool start at: %p\n", pool);

    printf("create pool: %ld\n", pool->max);


    // printf("mp_align(123, 32): %d, mp_align(17, 32): %d\n", mp_align(24, 32), mp_align(17, 32));
    // printf("mp_align_ptr(p->current, 32): %lx, p->current: %lx, mp_align(p->large, 32): %lx, p->large: %lx\n", mp_align_ptr(p->current, 32), p->current, mp_align_ptr(p->large, 32), p->large);

    int i = 0, j = 0;
    for (i = 0; i < 5; i++)
    {

        char *pp = mp_alloc(pool, 32);
        printf("pp: %p\n", pp);
        for (j = 0; j < 32; j++)
        {
            if (pp[j])
            {
                printf("alloc wrong\n");
            }
            printf("alloc success\n");
        }
    }


    for (i = 0; i < 5; i++)
    {
        void *l = mp_alloc(pool, 8192);
        printf("mempool alloc large: %p\n", l);
        mp_free(pool, l);
    }

    for (i = 0; i < 58; i++) {
        mp_alloc(pool, 256);
        printf("mempool alloc blcok\n");
    }

    mp_destroy(pool);

    return 0;
}
