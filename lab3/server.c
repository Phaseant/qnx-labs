#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

void reverse_wide_string(wchar_t *wstr) {
    int start = 0;
    int end = wcslen(wstr) - 1;
    while (start < end) {
        wchar_t temp = wstr[start];
        wstr[start] = wstr[end];
        wstr[end] = temp;
        start++;
        end--;
    }
}

void server(void) {
    int rcvid;
    int chid;
    char message[256];
    char response[512];
    wchar_t wide_message[256];

    setlocale(LC_ALL, "");  // Устанавливаем локаль для поддержки UTF-8

    chid = ChannelCreate(0);
    if (chid == -1) return;

    printf("Server started\n");
    printf("Channel ID: %d\n", chid);
    printf("Pid: %d\n", getpid());

    while (1) {
        rcvid = MsgReceive(chid, message, sizeof(message), NULL);
        if (rcvid == -1) continue;

        printf("Received request: '%s'\n", message);

        mbstowcs(wide_message, message, 256);
        reverse_wide_string(wide_message);
        wcstombs(message, wide_message, sizeof(message) - 1);
        message[sizeof(message) - 1] = '\0';


        snprintf(response, sizeof(response), "Reversed message: '%s'", message);

        MsgReply(rcvid, EOK, response, strlen(response) + 1);
        printf("Sent response: '%s'\n", response);
    }
}

int main(void) {
    server();
    return 0;
}
