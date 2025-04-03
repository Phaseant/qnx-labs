#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/siginfo.h>

#define MY_TIMER_CODE 10

int timer_setup(int chid);

int main(void)
{
    int chid;         // Идентификатор канала
    int rcvid;        // Идентификатор клиента
    struct _pulse pls; // Сообщение-импульс

    // Создание канала для приема импульсов
    if ((chid = ChannelCreate(0)) == -1)
    {
        perror("ChannelCreate");
        exit(EXIT_FAILURE);
    }

    // Создание и запуск таймера
    if (timer_setup(chid) == -1)
    {
        perror("timer_setup()");
        exit(EXIT_FAILURE);
    }

    // Цикл приема импульсов
    while (1)
    {
        MsgReceivePulse(chid, &pls, sizeof(pls), NULL);
        if (pls.code == MY_TIMER_CODE)
        {
            printf("Tick.\n");
        }
    }

    return EXIT_SUCCESS;
}

// Функция создает и запускает таймер
int timer_setup(int chid)
{
    timer_t tid;                   // Идентификатор таймера
    struct sigevent event;        // Описание уведомления от таймера
    struct itimerspec timer;      // Описание работы таймера
    int coid;                     // Идентификатор соединения

    // Подключение к каналу
    coid = ConnectAttach(0, 0, chid, 0, 0);
    if (coid == -1)
    {
        return -1;
    }

    // Настройка уведомления
    SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, MY_TIMER_CODE, 0);

    // Создание таймера
    if (timer_create(CLOCK_REALTIME, &event, &tid) == -1)
    {
        return -1;
    }

    // Настройка таймера
    timer.it_value.tv_sec = 3;     // Впервые сработает через 3 сек
    timer.it_value.tv_nsec = 0;
    timer.it_interval.tv_sec = 1;  // Периодичность: каждые 1 сек
    timer.it_interval.tv_nsec = 0;

    // Запуск таймера
    if (timer_settime(tid, 0, &timer, NULL) == -1)
    {
        return -1;
    }

    return 0;
}
