#include <iostream>
#include <thread>
#include <random>
#include <Windows.h>
#include <chrono>

const int ARRAY_SIZE = 10;
LONG arr[ARRAY_SIZE];
CRITICAL_SECTION cs;

void modifyArray(LPVOID) {
    srand(time(0));

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = rand() % 301 - 150;
    }
}

void squareNegativeNumbers(LPVOID) {
    while (true) {
        //вход в критическую сессию
        EnterCriticalSection(&cs);
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            if (arr[i] < 0) {
                arr[i] *= arr[i];
            }
        }
        //выход
        LeaveCriticalSection(&cs);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void printArray(LPVOID) {
    while (true) {
        EnterCriticalSection(&cs);
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
        LeaveCriticalSection(&cs);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    // инициализация к.с
    InitializeCriticalSection(&cs);
    modifyArray(nullptr);

    HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modifyArray, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)squareNegativeNumbers, NULL, 0, NULL);
    HANDLE hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)printArray, NULL, 0, NULL);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    WaitForSingleObject(hThread3, INFINITE);

    DeleteCriticalSection(&cs);
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);

    return 0;
}
