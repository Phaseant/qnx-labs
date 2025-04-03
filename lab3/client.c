#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char smsg[100];
    char rmsg[200];
    int coid;
    long serv_pid;

    printf("Client started\n");
    sleep(1);
    printf("Enter server PID:\n");
    scanf("%ld", &serv_pid);
    getchar();

    coid = ConnectAttach(0, serv_pid, 1, 0, 0);
    printf("coid=%d\n", coid);

    while (1) {
        sleep(1);
        printf("\nEnter your request (enter 'exit' for exit)\n");
        fgets(smsg, sizeof(smsg), stdin);
        if (!strcmp(smsg, "exit\n")) {
            ConnectDetach(coid);
            exit(0);
        }
        size_t len = strlen(smsg);
        if (len > 0 && smsg[len - 1] == '\n') {
            smsg[len - 1] = '\0';
        }
        printf("Your message: %s\n", smsg);
        if (MsgSend(coid, smsg, strlen(smsg) + 1, rmsg, sizeof(rmsg)) == -1) {
            ConnectDetach(coid);
            perror("Failed to send message");
            return 1;
        }
        printf("Received response: '%s'\n", rmsg);
    }

    ConnectDetach(coid);
    return (0);
}
