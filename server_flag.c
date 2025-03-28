#include "shared_memory.h"
volatile sig_atomic_t done = 0;
void signal_handler(int sig) { done = 1; }
void cleanup_resources() {
    sem_unlink(SEM_PRODUCER_NAME);
    sem_unlink(SEM_CONSUMER_NAME);
    shm_unlink(SHM_NAME);
    printf("Сервер: ресурсы освобождены\n");
}
int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigaction(SIGINT, &sa, NULL);
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_PRODUCER_NAME);
    sem_unlink(SEM_CONSUMER_NAME);
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
        perror("ftruncate");
        close(shm_fd);
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    SharedData *shared_data =
        (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    shared_data->ready = 0;
    shared_data->terminate = 0;
    sem_t *sem_producer = sem_open(SEM_PRODUCER_NAME, O_CREAT, 0666, 1);
    sem_t *sem_consumer = sem_open(SEM_CONSUMER_NAME, O_CREAT, 0666, 0);
    if (sem_producer == SEM_FAILED || sem_consumer == SEM_FAILED) {
        perror("sem_open");
        munmap(shared_data, sizeof(SharedData));
        close(shm_fd);
        shm_unlink(SHM_NAME);
        exit(EXIT_FAILURE);
    }
    printf("Сервер запущен. Нажмите Ctrl+C для завершения.\n");
    while (!done) {
        sem_wait(sem_consumer);
        if (shared_data->terminate || done) {
            printf("Сервер: получен сигнал завершения\n");
            break;
        }
        if (shared_data->ready) {
            printf("Сервер: получено число %d\n", shared_data->number);
            shared_data->ready = 0;
        }
        sem_post(sem_producer);
    }
    if (!shared_data->terminate) {
        sem_wait(sem_producer);
        shared_data->terminate = 1;
        sem_post(sem_consumer);
    }
    printf("Сервер: завершение работы...\n");
    sem_close(sem_producer);
    sem_close(sem_consumer);
    munmap(shared_data, sizeof(SharedData));
    close(shm_fd);
    cleanup_resources();
    return 0;
}
