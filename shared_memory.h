#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>  // Добавлено для использования errno

#define SHM_NAME "/my_shared_memory"
#define SEM_PRODUCER_NAME "/sem_producer"
#define SEM_CONSUMER_NAME "/sem_consumer"
#define SEM_TERMINATION_NAME "/sem_termination"

typedef struct {
    int number;         // Сгенерированное случайное число
    int ready;          // Флаг готовности (1 - число готово, 0 - нет)
    int terminate;      // Флаг завершения (1 - завершить работу, 0 - продолжать)
    pid_t client_pid;   // PID клиента для варианта с сигналами
} SharedData;

#endif
