#ifndef SHM_DATA_H
#define SHM_DATA_H

#define MESSAGE_MAX_SIZE 2048

#define FLAG_WRITE 1
#define FLAG_READ 2

typedef struct shmdata_s shmdata_t;

struct shmdata_s {
    int flag;   // 可读、可写
    char message[MESSAGE_MAX_SIZE];
};

#endif //SHM_DATA_H