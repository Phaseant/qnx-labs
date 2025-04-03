#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define MEM_NAME "ourmemory"

int threads(void);
void *thread_1(void *);
void *thread_2(void *);
int process_1(void);
int process_2(void);

// Структура разделяемой памяти
typedef struct {
    sem_t sem;
    int value;
} sh_mem_t;

pthread_mutex_t mut;      // Мьютекс
sh_mem_t *sm;             // Указатель на разделяемую память

int main(void)
{
    int md;               // Дескриптор разделяемой памяти
    int childpid;         // Идентификатор дочернего процесса

    /* ИНИЦИАЛИЗАЦИЯ */

    // На случай не удаленного блока памяти
    shm_unlink(MEM_NAME);

    // Создадим разделяемую память
    md = shm_open(MEM_NAME, O_RDWR | O_CREAT, 0777);
    if (md == -1) {
        perror("shm_open()");
        return -1;
    }

    // Установка размера
    if (ftruncate(md, sizeof(*sm)) == -1) {
        perror("ftruncate()");
        return -1;
    }

    // Разметка объекта памяти
    sm = (sh_mem_t *) mmap(NULL, sizeof(*sm), PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);
    if (sm == MAP_FAILED) {
        perror("mmap()");
        return -1;
    }

    // Инициализация семафора
    if (sem_init(&(sm->sem), 1, 1) == -1) {
        perror("sem_init()");
        exit(EXIT_FAILURE);
    }

    // Записываем начальное значение
    sm->value = 0;

    /* ИСПОЛНЕНИЕ */

    childpid = fork();
    if (childpid == -1) {
        perror("fork()");
    }

    if (childpid) {
        // Родительский процесс
        if (process_1()) {
            perror("process_1()");
            exit(EXIT_FAILURE);
        }

        // Ждём завершения дочерних процессов
        wait(NULL);
    } else {
        // Дочерний процесс
        if (process_2()) {
            perror("process_2()");
            exit(EXIT_FAILURE);
        }

        // Работаем с потоками
        if (threads()) {
            perror("threads()");
            exit(EXIT_FAILURE);
        }
    }

    /* ЗАВЕРШЕНИЕ */

    close(md); // Закроем разделяемую область
    shm_unlink(MEM_NAME); // Удалим её
    pthread_mutex_destroy(&mut); // Уничтожение мьютекса

    return EXIT_SUCCESS;
}

// Первый процесс
int process_1(void)
{
    // Блокируем память при помощи семафора
    sem_wait(&(sm->sem));
    printf("First process.\n(1) Previous value: %i\n", sm->value);

    // Работа с разделяемой памятью
    sm->value = 10;
    sm->value += 2;
    sm->value *= 4;
    sm->value -= 15;
    printf("(1) Result: %i\n", sm->value);

    // Разблокируем
    sem_post(&(sm->sem));

    return 0;
}

// Второй процесс
int process_2(void)
{
    // Берем ключ
    sem_wait(&(sm->sem));
    printf("Second process.\n(2) Previous value: %i\n", sm->value);

    // Работа с разделяемой памятью
    sm->value = 7;
    sm->value += 5;
    sm->value *= 2;
    sm->value -= 1;
    printf("(2) Result: %i\n", sm->value);

    // Возвращаем ключ
    sem_post(&(sm->sem));

    return 0;
}

// Создание двух потоков с мьютексом
int threads(void)
{
    pthread_t thid_1, thid_2;

    // Инициализация мьютекса
    if (pthread_mutex_init(&mut, NULL) == -1) {
        perror("pthread_mutex_init()");
        return -1;
    }

    // Создание потоков
    if (pthread_create(&thid_1, NULL, &thread_1, NULL) == -1 ||
        pthread_create(&thid_2, NULL, &thread_2, NULL) == -1) {
        perror("pthread_create()");
        return -1;
    }

    // Ждём завершения
    pthread_join(thid_1, NULL);
    pthread_join(thid_2, NULL);

    return 0;
}

// Первый поток
void *thread_1(void *arg)
{
    pthread_mutex_lock(&mut);
    printf("First lock\n");
    sleep(2);
    printf("First unlock\n");
    pthread_mutex_unlock(&mut);
    return NULL;
}

// Второй поток
void *thread_2(void *arg)
{
    pthread_mutex_lock(&mut);
    printf("Second lock\n");
    sleep(2);
    printf("Second unlock\n");
    pthread_mutex_unlock(&mut);
    return NULL;
}
