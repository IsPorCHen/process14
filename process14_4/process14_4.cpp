#include <iostream>
#include <thread>
#include <random>
#include <Windows.h>
#include <chrono>

const LONG SIZE_ARR = 10;
LONG arr[SIZE_ARR];     

void modifyArray(LPVOID) {
    srand(time(0));

    for (int i = 0; i < SIZE_ARR; ++i) {
        arr[i] = rand() % 301 - 150;
    }
}

void replacePositiveNumbers(LPVOID) {
    while (true) {
        for (int i = 0; i < SIZE_ARR; ++i) {
            if (arr[i] > 0) {
                InterlockedExchange(&arr[i], 0);
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void printArray(LPVOID) {
    while (true) {
        for (int i = 0; i < SIZE_ARR; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    modifyArray(nullptr);

    HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modifyArray, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)replacePositiveNumbers, NULL, 0, NULL);
    HANDLE hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)printArray, NULL, 0, NULL);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    WaitForSingleObject(hThread3, INFINITE);


    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);

    return 0;
}
