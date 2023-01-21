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
HANDLE h_Mutex, h_Barber, h_Customer; // �������
 // ����������� WINAPI ��� �������� �������
DWORD WINAPI CustomerThread(void* p);
DWORD WINAPI BaberThread(void* p);
DWORD WINAPI BaberThread(void* p)
{
    while (1)
    {
        WaitForSingleObject(h_Customer, -1); // ������� P
        WaitForSingleObject(h_Mutex, -1); // ������� P
        chairs++; // ������ + 1
        ReleaseSemaphore(h_Barber, 1, NULL); // ������� V
        ResumeThread((HANDLE)CustomerThread); // ����������� � �������� ��������
        ReleaseMutex(h_Mutex); // ������� V
        cout << "\n************* ���������� �������� ������!" << endl;
        Sleep(4000);
        cout << "\n************* ���������� ����������� �������!" << endl;
    }
    return 0;
}
DWORD WINAPI CustomerThread(void* p)
{
    int id = ++thread_id; // ID �������
    cout << "\n������ " << id << " ��������� ������ �� �������." << endl;
    WaitForSingleObject(h_Mutex, -1); // ������� P
    if (chairs > 0) // ���� ������� ��������
    {
        chairs--;
        cout << "\n������ " << id << " ������ � ��������� �������" << "\n����� ���������� ��������� ����� " << MAXCHAIR - chairs << endl;
        ReleaseSemaphore(h_Customer, 1, NULL);
        ResumeThread((HANDLE)BaberThread);
        ReleaseMutex(h_Mutex);

        cout << "\n�������� " << id << " ����������� �������." << endl;
        WaitForSingleObject(h_Barber, -1);
        cout << "\n������ " << id << " ���������." << endl;
        Sleep(3000);
        cout << "\n������ " << id << " ������� ������������� � ������." << endl;
    }
    else
    {
        cout << "\n������ " << id << " ��� �����, �����." << endl;
        ReleaseMutex(h_Mutex);
    }
    return 0;
}
void pro()
{
    // �������������
    h_Mutex = CreateMutex(NULL, FALSE, "mutex");
    h_Barber = CreateSemaphore(NULL, 0, 1, "barber");
    h_Customer = CreateSemaphore(NULL, 0, MAXCHAIR, "customer");

    HANDLE h_Thread;
    chairs = MAXCHAIR;
    cout << "������ ���������:" << endl;
    cout << endl;
    thread_id = 0;
    // ������� �������������� ����
    h_Thread = CreateThread(NULL, 0, BaberThread, NULL, 0, NULL);
    // �������� ���������� �������
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
    cout << "| 1. ������ ��������� |" << endl;
    cout << "| 2. ����� �� ��������� |" << endl;
    cout << endl;
    cout << "************************************" << endl;
start:
    cout << "����������, ������� �����, ������� �� ������ �������:" << endl;
    cin >> num;
    if (num < 1 || num>2)
    {
        cout << "������� �������� ������, ������� ������" << endl;
        goto start;
    }
    else if (num == 1)
    {
        cout << "�������� ����� �������� � ���������� ����:" << MAX_THREAD_NUM << " " << MAXCHAIR << "�������" << endl;
        pro();
    }
    else if (num == 2)
    {
        exit(0);
    }
    return 0;
}