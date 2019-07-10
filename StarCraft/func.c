#include "header.h"
//�ѹ� ���� �� �Լ� ����

uint64_t s[2];

uint64_t myRandom(void) {
	uint64_t x = s[0];
	uint64_t const y = s[1];
	s[0] = y;
	x ^= x << 23; // a
	s[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
	return s[1] + y;
}

void gotoxy(int x, int y) {	//gotoxy
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void initCursor() {	//Ŀ�� ��¦�� ����
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void fullscreenConsole() {	//��üȭ��
	system("cls");

	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

#ifndef _WIN64
	SetConsoleDisplayMode(hCon, CONSOLE_FULLSCREEN_MODE, NULL);
#else //_WIN64
	while (!IsWindowVisible(GetConsoleWindow()))
		Sleep(1);
	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
#endif

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hCon, &info);

	COORD bufSize;
	bufSize.X = info.dwMaximumWindowSize.X;
	bufSize.Y = info.dwMaximumWindowSize.Y;

	int ret = 0;
	for (int i = 0; i < 100; i++) {
		ret = SetConsoleScreenBufferSize(hCon, bufSize);
		if (ret)
			break;
	}
	if (ret == 0)
		abort();

	global_width = bufSize.X + 3;
	global_height = bufSize.Y;
}

/*
  �ܼ� �������� HWND �ڵ��� �����ɴϴ�.
  GetConsoleWindow() ��� �Լ��� ���� 10 ���彺�� 2 ������Ʈ ���� ��������,
  �� ���� ���������� ���� �����;� �մϴ�.
 */

HWND getConsoleWindowHandle() {
	WCHAR oldTitle[2048] = { 0 };
	WCHAR randTitle[2048] = { 0 };

	// ������ â ������ ����
	GetConsoleTitle(oldTitle, 2048);

	// �������� â ���� ����
	for (int i = 0; i < 2048; i++)
		randTitle[i] = myRandom() % ('Z' - 'A') + 'A';

	// �������� ������ �� �̸����� â ���� �ٲٱ�
	SetConsoleTitle(randTitle);

	// �����찡 ���� ������ ������Ʈ �� ���� ��ٸ�
	Sleep(300);

	// �� �̸��� â�� ã��
	HWND hWnd = FindWindow(NULL, randTitle);

	// �ٲٱ� ���� �̸����� ����
	SetConsoleTitle(oldTitle);
	return hWnd;
}




int GetDPI(HWND hWnd) {	//ȭ�� ���� ����
	HANDLE user32 = GetModuleHandle(TEXT("user32"));
	FARPROC func = GetProcAddress(user32, "GetDpiForWindow");
	if (func == NULL)
		return 96;
	return ((UINT(__stdcall *)(HWND))func)(hWnd);
}