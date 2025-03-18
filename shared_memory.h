#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#define SHM_NAME "/my_shared_memory"
#define SEM_PRODUCER_NAME "/sem_producer"
#define SEM_CONSUMER_NAME "/sem_consumer"
#define SEM_TERMINATION_NAME "/sem_termination"
typedef struct {
    int number;
    int ready;
    int terminate;
    pid_t client_pid;
} SharedData;
#endif
