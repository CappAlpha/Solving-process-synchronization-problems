#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <windows.h>
#include <io.h>
#include <conio.h>
#include  <stdlib.h>
#include <fstream>
#include <locale.h>
using namespace std;
#define MAXCHAIR 2
#define MAX_THREAD_NUM 6
int chairs, thread_id;
HANDLE h_Mutex, h_Barber, h_Customer; // Семафор
 // Используйте WINAPI для создания потоков
DWORD WINAPI CustomerThread(void* p);
DWORD WINAPI BaberThread(void* p);
DWORD WINAPI BaberThread(void* p)
{
    while (1)
    {
        WaitForSingleObject(h_Customer, -1); // Семафор P
        WaitForSingleObject(h_Mutex, -1); // семафор P
        chairs++; // пробел + 1
        ReleaseSemaphore(h_Barber, 1, NULL); // семафор V
        ResumeThread((HANDLE)CustomerThread); // Просыпаемся в ожидании клиентов
        ReleaseMutex(h_Mutex); // семафор V
        cout << "\n************* Парикмахер начинает резать!" << endl;
        Sleep(4000);
        cout << "\n************* Парикмахер заканчивает стрижку!" << endl;
    }
    return 0;
}
DWORD WINAPI CustomerThread(void* p)
{
    int id = ++thread_id; // ID клиента
    cout << "\nКлиент " << id << " Отправить заявку на стрижку." << endl;
    WaitForSingleObject(h_Mutex, -1); // семафор P
    if (chairs > 0) // Есть позиция ожидания
    {
        chairs--;
        cout << "\nКлиент " << id << " Сядьте и дождитесь стрижки" << "\nОбщее количество ожидающих людей " << MAXCHAIR - chairs << endl;
        ReleaseSemaphore(h_Customer, 1, NULL);
        ResumeThread((HANDLE)BaberThread);
        ReleaseMutex(h_Mutex);

        cout << "\nЗаказчик " << id << " Приготовить стрижку." << endl;
        WaitForSingleObject(h_Barber, -1);
        cout << "\nКлиент " << id << " Стрижется." << endl;
        Sleep(3000);
        cout << "\nКлиент " << id << " Стрижка заканчивается и уходит." << endl;
    }
    else
    {
        cout << "\nКлиент " << id << " Нет места, уходи." << endl;
        ReleaseMutex(h_Mutex);
    }
    return 0;
}
void pro()
{
    // Инициализация
    h_Mutex = CreateMutex(NULL, FALSE, "mutex");
    h_Barber = CreateSemaphore(NULL, 0, 1, "barber");
    h_Customer = CreateSemaphore(NULL, 0, MAXCHAIR, "customer");

    HANDLE h_Thread;
    chairs = MAXCHAIR;
    cout << "Начало программы:" << endl;
    cout << endl;
    thread_id = 0;
    // Создаем парикмахерскую нить
    h_Thread = CreateThread(NULL, 0, BaberThread, NULL, 0, NULL);
    // Создание клиентской цепочки
    for (int i = 0; i < MAX_THREAD_NUM; i++)
    {
        h_Thread = CreateThread(NULL, 0, CustomerThread, NULL, 0, NULL);
        Sleep(3000);
    }

    CloseHandle(h_Mutex);
    CloseHandle(h_Barber);
    CloseHandle(h_Customer);
    CloseHandle(h_Thread);
}
int main()
{
    setlocale(LC_ALL, "RUS");
    int num;
    int cnt = 0;
    cout << "************************************" << endl;
    cout << endl;
    cout << "| 1. Начало программы |" << endl;
    cout << "| 2. Выйти из программы |" << endl;
    cout << endl;
    cout << "************************************" << endl;
start:
    cout << "Пожалуйста, введите выбор, который вы хотите сделать:" << endl;
    cin >> num;
    if (num < 1 || num>2)
    {
        cout << "Введены неверные данные, введите заново" << endl;
        goto start;
    }
    else if (num == 1)
    {
        cout << "········ Число клиентов и количество мест:" << MAX_THREAD_NUM << " " << MAXCHAIR << "·······" << endl;
        pro();
    }
    else if (num == 2)
    {
        exit(0);
    }
    return 0;
}