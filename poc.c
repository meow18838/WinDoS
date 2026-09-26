#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <process.h>
#include <time.h>

#define NUM_THREADS 512
#define ITERATIONS 5000
#define DESKTOP_ALL_ACCESS 0x01FF

volatile LONG desktopsCreated = 0;
volatile LONG threadsRunning = 0;

unsigned int __stdcall DesktopRapidCreateCloseThread(void* arg) {
    int id = (int)(intptr_t)arg;
    for (int i = 0; i < ITERATIONS; i++) {
        char deskName[32];
        snprintf(deskName, sizeof(deskName), "Instant_%d_%d", id, i);

        HDESK hDesk = CreateDesktopA(deskName, NULL, NULL, 0, DESKTOP_ALL_ACCESS, NULL);
        if (hDesk) {
            CloseDesktop(hDesk);
        }
    }
    return 0;
}

int main() {
    HANDLE threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = (HANDLE)_beginthreadex(NULL, 0, DesktopRapidCreateCloseThread, (void*)(intptr_t)i, 0, NULL);
    }
}
