#include <windows.h>
#include <stdio.h>
#include "iostream"
using namespace std;
int Sklad = 0;
HANDLE ghSemaphore;
HANDLE c1, c2, p1, p2, p3;
unsigned long cid1, cid2, pid1, pid2, pid3;

void Thread_Consumer() // Клиент
{
	while (1)
	{
		WaitForSingleObject(ghSemaphore, INFINITE);
		while (Sklad < 1) //привязка 
		{
			printf("%d Покупатель ожидает\n", GetCurrentThreadId());
			Sleep(1);
		}
		Sklad--;
		printf("Сейчас состав имеет : %d\n\n", Sklad);
		Sleep(1000);
		ReleaseSemaphore(ghSemaphore, 1, NULL);
	}
}
void Thread_Producer() //Поставщик
{
	while (1)
	{
		WaitForSingleObject(ghSemaphore, INFINITE);
		while (Sklad > 4) // привязка
		{
			printf("%d Поставщик  %d ожидает\n", GetCurrentThreadId());
			Sleep(1);
		}
		Sklad++;
		//printf("Покуавтель купил\n\n", Sklad);
		printf("Сейчас состав имеет: %d\n\n", Sklad);
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
