#include <windows.h>
#include <stdio.h>
#include "iostream"
using namespace std;
int Sklad = 0;
HANDLE ghSemaphore;
HANDLE c1, c2, p1, p2, p3;
unsigned long cid1, cid2, pid1, pid2, pid3;

void Thread_Consumer() // ������
{
	while (1)
	{
		WaitForSingleObject(ghSemaphore, INFINITE);
		while (Sklad < 1) //�������� 
		{
			printf("%d ���������� �������\n", GetCurrentThreadId());
			Sleep(1);
		}
		Sklad--;
		printf("������ ������ ����� : %d\n\n", Sklad);
		Sleep(1000);
		ReleaseSemaphore(ghSemaphore, 1, NULL);
	}
}
void Thread_Producer() //���������
{
	while (1)
	{
		WaitForSingleObject(ghSemaphore, INFINITE);
		while (Sklad > 4) // ��������
		{
			printf("%d ���������  %d �������\n", GetCurrentThreadId());
			Sleep(1);
		}
		Sklad++;
		//printf("���������� �����\n\n", Sklad);
		printf("������ ������ �����: %d\n\n", Sklad);
		Sleep(1000);
		ReleaseSemaphore(ghSemaphore, 1, NULL);
	}
}
int main(void)
{
	setlocale(LC_ALL, "Rus");
	ghSemaphore = CreateSemaphore(NULL, 2, 5, NULL);
	c1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Consumer, NULL, 0, &cid1);
	c2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Consumer, NULL, 0, &cid2);
	p1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Producer, NULL, 0, &pid1);
	p2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Producer, NULL, 0, &pid2);
	p3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Producer, NULL, 0, &pid3);
	while (1);
	return 0;
}
