#include <windows.h>
#include <iostream>
#include <ctime>

using namespace std;

const int ARRAY_SIZE = 10;
long arr[ARRAY_SIZE];
CRITICAL_SECTION cs;
bool running = true;

void printArray() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

DWORD WINAPI threadFillArray(LPVOID) {
    srand(GetTickCount());

    while (running) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            long newValue = rand() % 301 - 150; 
            InterlockedExchange(&arr[i], newValue);
        }

        EnterCriticalSection(&cs);
        cout << "Массив до изменений: ";
        printArray();
        LeaveCriticalSection(&cs);
        Sleep(1000);
    }
    return 0;
}

DWORD WINAPI threadCriticalSection(LPVOID) {
    while (running) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            long oldValue = arr[i];
            if (oldValue < 0) {
                long newValue = oldValue * oldValue;
                EnterCriticalSection(&cs);
                InterlockedCompareExchange(&arr[i], newValue, oldValue);
                LeaveCriticalSection(&cs);
            }
        }

        EnterCriticalSection(&cs);
        cout << "Массив после изменений: ";
        printArray();
        LeaveCriticalSection(&cs);

        Sleep(1000);
    }
    return 0;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    InitializeCriticalSection(&cs);

    HANDLE hThreads[2];

    hThreads[0] = CreateThread(NULL, 0, threadFillArray, NULL, 0, NULL);
    hThreads[1] = CreateThread(NULL, 0, threadCriticalSection, NULL, 0, NULL);

    if (!hThreads[0] || !hThreads[1]) {
        std::cerr << "Ошибка при создании потоков!" << std::endl;
        DeleteCriticalSection(&cs);
        return 1;
    }

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
    for (HANDLE hThread : hThreads) {
        CloseHandle(hThread);
    }

    DeleteCriticalSection(&cs);
    return 0;
}
