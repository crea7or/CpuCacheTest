// CacheFalseSharing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>

#define THREADS_CNT 4
#define ARRAY_SIZE 40000

DWORD distance = 1;
DWORD array[ ARRAY_SIZE ];

#pragma optimize( "", off )
DWORD WINAPI increaseArray(LPVOID lpParam)
{
	DWORD index = *(LPDWORD)lpParam;
	for (DWORD current = 0; current < 30000000; ++current)
	{
		array[index * distance] += current++;
	}
	return 0;
}
#pragma optimize( "", on )

void Tester( LPTHREAD_START_ROUTINE func )
{
	HANDLE threads[THREADS_CNT];
	DWORD treadIds[THREADS_CNT];
	DWORD treadValues[THREADS_CNT];

	DWORD startTicks = GetTickCount();

	for (DWORD thr = 0; thr < THREADS_CNT; ++thr)
	{
		treadValues[thr] = thr;
		threads[thr] = CreateThread(NULL, 0, func, &treadValues[thr], 0, &treadIds[thr]);
	}

	DWORD threadsRes = WaitForMultipleObjects(THREADS_CNT, (const HANDLE*)&threads, TRUE, 10000);

	DWORD resultTicks = GetTickCount() - startTicks;

	if (threadsRes > WAIT_OBJECT_0 + THREADS_CNT)
	{
		printf("Error: %d\n", threadsRes);
	}
	else
	{
		printf("Result: %d\n", resultTicks );
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	distance = 0;
	printf("\nTests will be performed simultaneously in %d threads.\n", THREADS_CNT);
	printf("\nModification of the same variable in array.\nDistance between values that are changes %d.\n", distance );
	for (DWORD i = 0; i < 5; i++)
	{
		Tester(increaseArray);
	}

	distance = 1;
	printf("\nModification of the different variables that are very near to each other in the same array.\nThey will be placed in the same CPU cache line.\nDistance between values that are changes %d.\n", distance);
	for (DWORD i = 0; i < 5; i++ )
	{
		Tester(increaseArray);
	}

	distance = ARRAY_SIZE / 4;
	printf("\nModification of the different variables that are very far from each other in the same array.\nThey will be placed in the different CPU cache lines (I hope).\nDistance between values that are changes %d.\n", distance);
	for (DWORD i = 0; i < 5; i++)
	{
		Tester(increaseArray);
	}
}
