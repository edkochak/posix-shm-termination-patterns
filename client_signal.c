#include "shared_memory.h"
volatile sig_atomic_t done = 0;
void signal_handler(int sig) { done = 1; }
int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigaction(SIGTERM, &sa, NULL);
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) {
            perror("shm_open");
            exit(EXIT_FAILURE);
        }
        if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
            perror("ftruncate");
            exit(EXIT_FAILURE);
        }
    }
    SharedData *shared_data =
        (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    sem_t *sem_producer = sem_open(SEM_PRODUCER_NAME, O_CREAT, 0666, 1);
    sem_t *sem_consumer = sem_open(SEM_CONSUMER_NAME, O_CREAT, 0666, 0);
    if (sem_producer == SEM_FAILED || sem_consumer == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    shared_data->client_pid = getpid();
    srand(time(NULL));
    printf("Клиент запущен (PID: %d). Нажмите Ctrl+C для завершения.\n", getpid());
    while (!done) {
        sem_wait(sem_producer);
        if (done) break;
        shared_data->number = rand() % 100;
        shared_data->ready = 1;
        printf("Клиент: сгенерировано число %d\n", shared_data->number);
        sem_post(sem_consumer);
        usleep(500000);
    }
    printf("Клиент: завершение работы...\n");
    sem_close(sem_producer);
    sem_close(sem_consumer);
    munmap(shared_data, sizeof(SharedData));
    close(shm_fd);
    return 0;
}
