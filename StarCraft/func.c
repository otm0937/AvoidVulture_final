#include "header.h"
//한번 쓰고 말 함수 선언

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

void initCursor() {	//커서 반짝임 막기
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void fullscreenConsole() {	//전체화면
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
  콘솔 윈도우의 HWND 핸들을 가져옵니다.
  GetConsoleWindow() 라는 함수가 윈도 10 레드스톤 2 업데이트 에서 생겼지만,
  그 이하 버전에서는 직접 가져와야 합니다.
 */

HWND getConsoleWindowHandle() {
	WCHAR oldTitle[2048] = { 0 };
	WCHAR randTitle[2048] = { 0 };

	// 지금의 창 제목을 저장
	GetConsoleTitle(oldTitle, 2048);

	// 랜덤으로 창 제목 생성
	for (int i = 0; i < 2048; i++)
		randTitle[i] = myRandom() % ('Z' - 'A') + 'A';

	// 랜덤으로 생성한 그 이름으로 창 제목 바꾸기
	SetConsoleTitle(randTitle);

	// 윈도우가 관련 변수를 업데이트 할 동안 기다림
	Sleep(300);

	// 그 이름의 창을 찾음
	HWND hWnd = FindWindow(NULL, randTitle);

	// 바꾸기 전의 이름으로 복귀
	SetConsoleTitle(oldTitle);
	return hWnd;
}




int GetDPI(HWND hWnd) {	//화면 배율 측정
	HANDLE user32 = GetModuleHandle(TEXT("user32"));
	FARPROC func = GetProcAddress(user32, "GetDpiForWindow");
	if (func == NULL)
		return 96;
	return ((UINT(__stdcall *)(HWND))func)(hWnd);
}