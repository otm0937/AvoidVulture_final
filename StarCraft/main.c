#include "header.h"
#include "terran.h"
#include "UI.h"
#include "Sound.h"




DWORD __stdcall Soundmain(void*p);



void processInput()	// input ����
{
	if (GetAsyncKeyState(0x31) < 0) { //key 1
		Units[0].selected = 1;
		Units[1].selected = 0;
		Units[2].selected = 0;
	}
	else if (GetAsyncKeyState(0x32) < 0 && unitnum >= 2) { //key 2
		Units[0].selected = 0;
		Units[1].selected = 1;
		Units[2].selected = 0;
	}
	else if (GetAsyncKeyState(0x33) < 0 && unitnum >= 3) { //key 3
		Units[0].selected = 0;
		Units[1].selected = 0;
		Units[2].selected = 1;
	}

	if (GetAsyncKeyState(VK_LBUTTON) < 0) { // ��Ŭ��
		mouseMode = drag;
		click++;
		if (click == 1) {	//�巡�� ����
			start_x = GDImouse_X + scroll_X;
			start_y = GDImouse_Y + scroll_Y;
		}
	}
	else if (abs(start_x - GDImouse_X - scroll_X) < 15 && abs(start_y - GDImouse_Y - scroll_Y) < 15 && click > 0) {	//Ŭ��
		int c = 0;
		for (int i = 0; i < Unitcnt; i++) {
			if (Units[i].x - Units[i].sizeX / 2 - 5 < GDImouse_X + scroll_X && Units[i].x + Units[i].sizeX / 2 + 5 > GDImouse_X + scroll_X && Units[i].y - Units[i].sizeY / 2 - 5 < GDImouse_Y + scroll_Y && Units[i].y + Units[i].sizeY / 2 + 5 > GDImouse_Y + scroll_Y) {
				if (c == 0) {
					for (int aa = 0; aa < Unitcnt; aa++)   Units[aa].selected = 0;
				}
				Units[i].selected = 1;
				c = 1;
			}
		}
		click = 0;
		c = 0;
	}
	else if (click > 0)	//�巡�� ��
	{
		int mid_X = (start_x + GDImouse_X + scroll_X) / 2;
		int mid_Y = (start_y + GDImouse_Y + scroll_Y) / 2;
		int dis_X = abs(mid_X - start_x);
		int dis_Y = abs(mid_Y - start_y);
		int c = 0;
		for (int i = 0; i < Unitcnt; i++) {

			if (mid_X - dis_X < Units[i].x + 10 && mid_X + dis_X > Units[i].x - 10 && mid_Y - dis_Y < Units[i].y + 10 && mid_Y + dis_Y > Units[i].y - 10) {
				Units[i].selected = 0;
				if (c == 0) {
					for (int aa = 0; aa < Unitcnt; aa++)   Units[aa].selected = 0;
				}
				Units[i].selected = 1;
				c = 1;
			}
		}
		click = 0;
		c = 0;
	}

	if (GetAsyncKeyState(VK_RBUTTON) < 0) {	//��Ŭ��
		mouseMode = targg;
		int prev = -1, destx = 0, desty = 0;
		int prevnum = 0;
		for (int i = 0; i < Unitcnt; i++) {
			if (Units[i].selected == 1 && Units[i].temp == 0) {
				Units[i].temp = 1;	//�ڴ����� ������
				Units[i].moving = 1;
				Units[i].moveStep = 0;
				if (1) {
					//initMove(Units[i].x, Units[i].y, scroll_X + GDImouse_X, scroll_Y + GDImouse_Y, i);
					destx = scroll_X + GDImouse_X, desty = scroll_Y + GDImouse_Y;
					initLinearMove(Units[i].x, Units[i].y, destx, desty, i);
					Units[i].destX = destx, Units[i].destY = desty;
				}
				prev = i;
			}
		}
	}
	else {
		for (int i = 0; i < Unitcnt; i++) {
			Units[i].temp = 0;
		}
	}
}

DWORD __stdcall mouseInput(void *param)	//���콺 �Է�
{
	HWND hWnd = *(HWND*)param;
	POINT p;
	while (1)
	{
		GetCursorPos(&p);
		ScreenToClient(hWnd, &p);

		mouse_X = p.x;
		mouse_Y = p.y;
		GDImouse_X = (mouse_X - HorizonBorder) * 640 / (pixelWidth - HorizonBorder * 2);
		GDImouse_Y = (mouse_Y - VerticalBorder) * 480 / (pixelHeight - VerticalBorder * 2);
	}
}


int createUnit(int name, int x, int y) {	//���� ����
	for (int i = 0; i < Unitcnt; i++) {
		if (Units[i].isUsed == 0) {
			switch (name) {
			case Medic: Units[i] = MedicUnit;
			break;
			}
			Units[i].isUsed = 1;
			Units[i].x = x;
			Units[i].y = y;
			return 0;
		}
	}
	return -1;
}

int createEnemy(int name, int x, int y, int dir) {	//�� ����
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].isUsed == 0) {
			switch (name) {
			case Ghost: Enemy[i] = GhostUnit;
			break; case Vulture: Enemy[i] = VultureUnit;
			break; case BattleCruiser: Enemy[i] = BattleCruiserUnit;
			break; case ScienceVessel: Enemy[i] = ScienceVesselUnit;
			break;
			}
			Enemy[i].isUsed = 1;
			Enemy[i].x = x;
			Enemy[i].y = y;
			Enemy[i].direction = dir;
			if (Enemy[i].direction == 8 || Enemy[i].direction == 24) {
				int temp = Enemy[i].sizeX;
				Enemy[i].sizeX = Enemy[i].sizeY;
				Enemy[i].sizeY = temp;
			}
			return 0;
		}
	}
	return -1;
}

void initxy() {	//�����ȵ����� xy�� �ʱ�ȭ
	for (int i = 0; i < Unitcnt; i++) {
		if (Units[i].isUsed == 0) {
			Units[i].x = -9999;
			Units[i].y = -9999;
		}
	}
}

int collisionCheck(int i) {	//�浹 üũ

	for (int y = -Units[i].sizeY / 2-1; y <= Units[i].sizeY / 2; y++) {
		for (int x = -Units[i].sizeX / 2-1; x <= Units[i].sizeX / 2; x++) {
			if (Units[i].x + x <= 0 || Units[i].y + y <= 0 || Units[i].x + x >= 4096 || Units[i].y + y >= 4096)	continue;
			if (x >= -Units[i].sizeX / 2 && x < Units[i].sizeX / 2 && y >= -Units[i].sizeY && y < Units[i].sizeY / 2)	continue;
			if (fullMap[Units[i].x + x][Units[i].y + y] == 1 || fullMap[Units[i].x + x][Units[i].y + y] == 4) {
				return 1;
			}
		}
	}
	return 0;
}

int checkDirection(int x, int y, int destx, int desty) {	//���� üũ
	double g = (double)(desty - y) / (double)(destx - x); // g : ����
	g *= -1;
	if (destx == x) {
		if (desty > y)	return 16;
		else	return 0;
	}
	else if (-0.125000 <= g && g <= 0.125000)            return (destx > x ? 8 : 24);
	else if (-0.125000 >= g && g >= -0.375000)    return (destx > x ? 9 : 25);
	else if (-0.375000 >= g && g >= -0.625000)    return (destx > x ? 10 : 26);
	else if (-0.625000 >= g && g >= -0.875000)    return (destx > x ? 11 : 27);
	else if (-0.875000 >= g && g >= -1.143000)    return (destx > x ? 12 : 28);
	else if (-1.143000 >= g && g >= -1.600000)    return (destx > x ? 13 : 29);
	else if (-1.600000 >= g && g >= -2.667000)    return (destx > x ? 14 : 30);
	else if (-2.667000 >= g && g >= -8.000000)    return (destx > x ? 15 : 31);
	else if (8 <= g || g < -8)                    return (desty > y ? 16 : 0);
	else if (2.667000 <= g && g <= 8.000000)        return (destx > x ? 1 : 17);
	else if (1.600000 <= g && g <= 2.667000)        return (destx > x ? 2 : 18);
	else if (1.143000 <= g && g <= 1.600000)        return (destx > x ? 3 : 19);
	else if (0.875000 <= g && g <= 1.143000)        return (destx > x ? 4 : 20);
	else if (0.625000 <= g && g <= 0.875000)        return (destx > x ? 5 : 21);
	else if (0.375000 <= g && g <= 0.625000)        return (destx > x ? 6 : 22);
	else if (0.125000 <= g && g <= 0.375000)        return (destx > x ? 7 : 23);
}

void eraseScreen() {	//��ũ�� �����
	gotoxy(0, 0);
	for (int i = 0; i < 1010; i++)
		printf("             ");
	return;
}


void LinearMove() {	//������
	int x = 4;
	for (int i = 0; i < Unitcnt; i++) {
		if (Units[i].moveStep == 0) {
			Units[i].moving = 0;
		}
		else {
			Units[i].moveStep--;
			if (Units[i].route[Units[i].moveStep][0] < 1 || Units[i].route[Units[i].moveStep][0] > 640)	continue;
			if (Units[i].route[Units[i].moveStep][1] < 1 || Units[i].route[Units[i].moveStep][1] > 480)	continue;
			Units[i].x = Units[i].route[Units[i].moveStep][0];
			Units[i].y = Units[i].route[Units[i].moveStep][1];
			if (Units[i].moveStep != 0) {

				if (Units[i].route[Units[i].moveStep][0] < 1 || Units[i].route[Units[i].moveStep][0] > 640)	continue;
				if (Units[i].route[Units[i].moveStep][1] < 1 || Units[i].route[Units[i].moveStep][1] > 480)	continue;
				Units[i].moveStep--;
				Units[i].x = Units[i].route[Units[i].moveStep][0];
				Units[i].y = Units[i].route[Units[i].moveStep][1];
			}
		}

		if (Enemy[i].isUsed == 1) {
			if (Enemy[i].x < 0 || Enemy[i].y < 0 || Enemy[i].x > 700 || Enemy[i].y > 500) {
				Enemy[i].isUsed = 0;
			}

			else if (Enemy[i].name == Vulture) {
				if (Enemy[i].direction == 8) {
					Enemy[i].x += (difficulty + 1);
					Enemy[i].sizeX = 47;
					Enemy[i].sizeY = 20;
				}
				else if (Enemy[i].direction == 16)
					Enemy[i].y += (difficulty + 1);
				else if (Enemy[i].direction == 0)
					Enemy[i].y -= (difficulty + 1);
				else if (Enemy[i].direction == 24) {
					Enemy[i].sizeX = 50;
					Enemy[i].sizeY = 15;
					Enemy[i].x -= (difficulty + 1);
				}
			}

			else if (Enemy[i].name == BattleCruiser && score % 4 == 0) {
				Enemy[i].y++;
			}
		}

	}
}

int initLinearMove(int start_x, int start_y, int end_x, int end_y, int idx) {	//��ã�� (����)
	int dx = abs(end_x - start_x);
	int dy = abs(end_y - start_y);
	if (dy <= dx) {
		int p = 2 * (dy - dx);
		int y = start_y;

		int inc_x = 1;
		if (end_x < start_x)
		{
			inc_x = -1;
		}
		int inc_y = 1;
		if (end_y < start_y)
		{
			inc_y = -1;
		}
		for (int x = start_x; (start_x <= end_x ? x <= end_x : x >= end_x); x += inc_x)
		{
			Units[idx].route[Units[idx].moveStep][0] = x;
			Units[idx].route[Units[idx].moveStep][1] = y;
			Units[idx].moveStep++;
			if (0 >= p)
			{
				p += 2 * dy;
			}
			else
			{
				p += 2 * (dy - dx);
				y += inc_y;
			}
		}
	}
	else
	{
		int p = 2 * (dx - dy);
		int x = start_x;

		int inc_x = 1;
		if (end_x < start_x)
		{
			inc_x = -1;
		}
		int inc_y = 1;
		if (end_y < start_y)
		{
			inc_y = -1;
		}
		for (int y = start_y; (start_y <= end_y ? y <= end_y : y >= end_y); y += inc_y)
		{
			Units[idx].route[Units[idx].moveStep][0] = x;
			Units[idx].route[Units[idx].moveStep][1] = y;
			Units[idx].moveStep++;
			if (0 >= p)
			{
				p += 2 * dx;
			}
			else
			{
				p += 2 * (dx - dy);
				x += inc_x;
			}
		}
	}
	for (int i = 0; i < Units[idx].moveStep / 2; i++) {
		int temp = Units[idx].route[i][0];
		Units[idx].route[i][0] = Units[idx].route[Units[idx].moveStep - i - 1][0];
		Units[idx].route[Units[idx].moveStep - i][0] = temp;
		temp = Units[idx].route[i][1];
		Units[idx].route[i][1] = Units[idx].route[Units[idx].moveStep - i - 1][1];
		Units[idx].route[Units[idx].moveStep - i][1] = temp;
	}
	Units[idx].direction = checkDirection(start_x, start_y, end_x, end_y) / 2 * 2;
}


int gameContinue() {	//����� ���� Ȯ��


	gotoxy(60, 12);
	printf("  ������������������������������������������������������������������������������������������������������������������������ ");
	gotoxy(60, 13);
	printf("  ��                                                          �� ");
	gotoxy(60, 14);
	printf("  ��                                                          �� ");
	gotoxy(60, 15);
	printf("  ��                                                          �� ");
	gotoxy(60, 16);
	printf("  ��                        ���� ����                         �� ");
	gotoxy(60, 17);
	printf("  ��                                                          �� ");
	gotoxy(60, 18);
	printf("  ��                                                          �� ");
	gotoxy(60, 19);
	printf("  ��                 �ٽ� �÷����Ͻðڽ��ϱ�?                 �� ");
	gotoxy(60, 20);
	printf("  ��                                                          �� ");
	gotoxy(60, 21);
	printf("  ��                                                          �� ");
	gotoxy(60, 22);
	printf("  ��                                                          �� ");
	gotoxy(60, 23);
	printf("  ������������������������������������������������������������������������������������������������������������������������ ");
	k = 1;
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0)	k = 0;

		if (mouse_X >= 240 && mouse_X <= 710 && mouse_Y >= 570 && mouse_Y <= 690)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(27, 35);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(27, 36);
		printf("  ��                                                          �� ");
		gotoxy(27, 37);
		printf("  ��                                                          �� ");
		gotoxy(27, 38);
		printf("  ��                                                          �� ");
		gotoxy(27, 39);
		printf("  ��                       1. �ٽ� �ϱ�                       �� ");
		gotoxy(27, 40);
		printf("  ��                                                          �� ");
		gotoxy(27, 41);
		printf("  ��                                                          �� ");
		gotoxy(27, 42);
		printf("  ��                                                          �� ");
		gotoxy(27, 43);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		if (mouse_X >= 770 && mouse_X <= 1240 && mouse_Y >= 570 && mouse_Y <= 690)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(93, 35);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(93, 36);
		printf("  ��                                                          �� ");
		gotoxy(93, 37);
		printf("  ��                                                          �� ");
		gotoxy(93, 38);
		printf("  ��                                                          �� ");
		gotoxy(93, 39);
		printf("  ��                       2. ���� ����                       �� ");
		gotoxy(93, 40);
		printf("  ��                                                          �� ");
		gotoxy(93, 41);
		printf("  ��                                                          �� ");
		gotoxy(93, 42);
		printf("  ��                                                          �� ");
		gotoxy(93, 43);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(0, 0);

		if (GetAsyncKeyState(0x31) < 0 || (mouse_X >= 240 && mouse_X <= 710 && mouse_Y >= 570 && mouse_Y <= 690 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0)) {
			return 1;
		}

		else if (GetAsyncKeyState(0x32) < 0 || (mouse_X >= 770 && mouse_X <= 1240 && mouse_Y >= 570 && mouse_Y <= 690 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0)) {
			return 0;
		}

		if (GetAsyncKeyState(VK_LBUTTON) < 0)	k = 1;
	}
}


void difSel() {	//���̵� ����
	int k = 1;
	Sleep(200);
	initCursor();
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0) k = 0;
		gotoxy(85, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("���̵��� �����ϼ���.\n");
		gotoxy(0, 17);
		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("  ������������������������������������������������������������������������������������������������������������������������ \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                        1.����                            �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                    - ���� �ӵ� ����                      �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                    - ������ ����                         �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                    - ���� ���� ����                      �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��             ���� ���� ���̵�. �����̴� �����            �� \n");
		printf("  ��             �ͼ����� �ʰų� ���ϱ���� ������            �� \n");
		printf("  ��             �� ���ϴ� �ʽ��� ���� ���̵�.                �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		
		if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(63, 17);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(63, 18);
		printf("  ��                                                          �� ");
		gotoxy(63, 19);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 20);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 21);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 22);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 23);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 24);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 25);															 
		printf("  ��                         2. ����                          �� ");
		gotoxy(63, 26);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 27);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 28);															 
		printf("  ��                     - ���� �ӵ� ����                     �� ");
		gotoxy(63, 29);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 30);															 
		printf("  ��                     - ������ ����                        �� ");
		gotoxy(63, 31);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 32);															 
		printf("  ��                     - ���� ���̵� ����                   �� ");
		gotoxy(63, 33);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 34);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 35);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 36);															 
		printf("  ��                ���� ���̵�. ���� ���̵���                �� ");
		gotoxy(63, 37);															 
		printf("  ��                ���� ������ ������ ���� ��              �� ");
		gotoxy(63, 38);															 
		printf("  ��                �ִ� ���̵�.                              �� ");
		gotoxy(63, 39);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 40);															 
		printf("  ��                                                          �� ");
		gotoxy(63, 41);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");


		if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(126, 17);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(126, 18);
		printf("  ��                                                          �� ");
		gotoxy(126, 19);
		printf("  ��                                                          �� ");
		gotoxy(126, 20);
		printf("  ��                                                          �� ");
		gotoxy(126, 21);
		printf("  ��                                                          �� ");
		gotoxy(126, 22);
		printf("  ��                                                          �� ");
		gotoxy(126, 23);
		printf("  ��                                                          �� ");
		gotoxy(126, 24);
		printf("  ��                                                          �� ");
		gotoxy(126, 25);
		printf("  ��                         3. �����                        �� ");
		gotoxy(126, 26);
		printf("  ��                                                          �� ");
		gotoxy(126, 27);
		printf("  ��                                                          �� ");
		gotoxy(126, 28);
		printf("  ��                     - ���� �ӵ� ����                     �� ");
		gotoxy(126, 29);
		printf("  ��                                                          �� ");
		gotoxy(126, 30);
		printf("  ��                     - ������ ����                        �� ");
		gotoxy(126, 31);
		printf("  ��                                                          �� ");
		gotoxy(126, 32);
		printf("  ��                     - ���� ���̵� �����                 �� ");
		gotoxy(126, 33);
		printf("  ��                                                          �� ");
		gotoxy(126, 34);
		printf("  ��                                                          �� ");
		gotoxy(126, 35);
		printf("  ��                                                          �� ");
		gotoxy(126, 36);
		printf("  ��                �׳� ����� ����� ���̵�.                �� ");
		gotoxy(126, 37);
		printf("  ��                ���Ͽ� ���� ���� ���ؿ�                   �� ");
		gotoxy(126, 38);
		printf("  ��                �Ǹ��� ��Ʈ���� �־��                    �� ");
		gotoxy(126, 39);
		printf("  ��                Ŭ���� ������ ���̵�.                     �� ");
		gotoxy(126, 40);
		printf("  ��                                                          �� ");
		gotoxy(126, 41);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");

		if (GetAsyncKeyState(0x31) < 0) {
			difficulty = 1;
			break;
		}
		else if (GetAsyncKeyState(0x32) < 0) {
			difficulty = 2;
			break;
		}
		else if (GetAsyncKeyState(0x33) < 0) {
			difficulty = 3;
			break;
		}


		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k==0) {
			difficulty = 1;
			break;
		}
		else if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k==0) {
			difficulty = 2;
			break;
		}
		else if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k==0){
			difficulty = 3;
			break;
		}

		if (GetAsyncKeyState(VK_LBUTTON) < 0) k = 1;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	eraseScreen();

	gotoxy(60, 12);
	printf("  ������������������������������������������������������������������������������������������������������������������������ ");
	gotoxy(60, 13);
	printf("  ��                                                          �� ");
	gotoxy(60, 14);
	printf("  ��                                                          �� ");
	gotoxy(60, 15);
	printf("  ��                                                          �� ");
	gotoxy(60, 16);
	switch (difficulty) {
	case 1:
		printf("  ��               '����' ���̵��� ���õǾ����ϴ�.            �� ", difficulty);
		break;
	case 2:
		printf("  ��               '����' ���̵��� ���õǾ����ϴ�.            �� ", difficulty);
		break;
	case 3:
		printf("  ��               '�����' ���̵��� ���õǾ����ϴ�.          �� ", difficulty);
		break;
	}
	gotoxy(60, 17);
	printf("  ��                                                          �� ");
	gotoxy(60, 18);
	printf("  ��                                                          �� ");
	gotoxy(60, 19);
	printf("  ��                                                          �� ");
	gotoxy(60, 20);
	printf("  ������������������������������������������������������������������������������������������������������������������������ ");
	Sleep(1000);
	eraseScreen();
	initCursor();
	k = 1;
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0) k = 0;
		gotoxy(85, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("���̵��� �����ϼ���.\n");
		gotoxy(0, 17);
		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("  ������������������������������������������������������������������������������������������������������������������������ \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                        1.���� 1��                        �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                ���� �Ϲ����� �÷��� ���.                �� \n");
		printf("  ��                �ܼ��� ���ϱ� ������ ����               �� \n");
		printf("  ��                �ʹٸ�, �� ����� ����                    �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");

		if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(63, 17);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(63, 18);
		printf("  ��                                                          �� ");
		gotoxy(63, 19);
		printf("  ��                                                          �� ");
		gotoxy(63, 20);
		printf("  ��                                                          �� ");
		gotoxy(63, 21);
		printf("  ��                                                          �� ");
		gotoxy(63, 22);
		printf("  ��                                                          �� ");
		gotoxy(63, 23);
		printf("  ��                                                          �� ");
		gotoxy(63, 24);
		printf("  ��                                                          �� ");
		gotoxy(63, 25);
		printf("  ��                         2. ���� 2��                      �� ");
		gotoxy(63, 26);
		printf("  ��                                                          �� ");
		gotoxy(63, 27);
		printf("  ��                                                          �� ");
		gotoxy(63, 28);
		printf("  ��                                                          �� ");
		gotoxy(63, 29);
		printf("  ��                                                          �� ");
		gotoxy(63, 30);
		printf("  ��                ���� 2��� ���ĸ� ���ϴ� ���.            �� ");
		gotoxy(63, 31);
		printf("  ��                �ڽ��� ��Ƽ�½�ŷ �ɷ��� ���ų�           �� ");
		gotoxy(63, 32);
		printf("  ��                ���� 1��� Ŭ����� �ʹ� ���ٰ�           �� ");
		gotoxy(63, 33);
		printf("  ��                ������ �� �÷����ϸ� �� ���̵�.           �� ");
		gotoxy(63, 34);
		printf("  ��                                                          �� ");
		gotoxy(63, 35);
		printf("  ��                                                          �� ");
		gotoxy(63, 36);
		printf("  ��                                                          �� ");
		gotoxy(63, 37);				                                 
		printf("  ��                                                          �� ");
		gotoxy(63, 38);				                                 
		printf("  ��                                                          �� ");
		gotoxy(63, 39);
		printf("  ��                                                          �� ");
		gotoxy(63, 40);
		printf("  ��                                                          �� ");
		gotoxy(63, 41);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");


		if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(126, 17);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(126, 18);
		printf("  ��                                                          �� ");
		gotoxy(126, 19);
		printf("  ��                                                          �� ");
		gotoxy(126, 20);
		printf("  ��                                                          �� ");
		gotoxy(126, 21);
		printf("  ��                                                          �� ");
		gotoxy(126, 22);
		printf("  ��                                                          �� ");
		gotoxy(126, 23);
		printf("  ��                                                          �� ");
		gotoxy(126, 24);
		printf("  ��                                                          �� ");
		gotoxy(126, 25);
		printf("  ��                         3. ���� 3��                      �� ");
		gotoxy(126, 26);
		printf("  ��                                                          �� ");
		gotoxy(126, 27);
		printf("  ��                                                          �� ");
		gotoxy(126, 28);
		printf("  ��                                                          �� ");
		gotoxy(126, 29);
		printf("  ��                                                          �� ");
		gotoxy(126, 30);
		printf("  ��                �ϵ��ھ� ���̵�. �ƹ��� ���ص�            �� ");
		gotoxy(126, 31);
		printf("  ��                �� ������ ���̵��� Ŭ���� �� ��           �� ");
		gotoxy(126, 32);
		printf("  ��                ������ �ǽɵǱ� �����ϴ� ���̵�.          �� ");
		gotoxy(126, 33);
		printf("  ��                Ŭ�����ϸ�, �����ϰ� ���ΰ��̸�           �� ");
		gotoxy(126, 34);
		printf("  ��                ������ ����� ���� �� ����.               �� ");
		gotoxy(126, 35);
		printf("  ��                                                          �� ");
		gotoxy(126, 36);
		printf("  ��                                                          �� ");
		gotoxy(126, 37);			                           
		printf("  ��                                                          �� ");
		gotoxy(126, 38);			                           
		printf("  ��                                                          �� ");
		gotoxy(126, 39);			                           
		printf("  ��                                                          �� ");
		gotoxy(126, 40);
		printf("  ��                                                          �� ");
		gotoxy(126, 41);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");

		if (mouse_X >= 1146 && mouse_X <= 1390 && mouse_Y >= 740 && mouse_Y <= 840)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(140, 46);
		printf("  ���������������������������������������������������������������� ");
		gotoxy(140, 47);
		printf("  ��                              �� ");
		gotoxy(140, 48);
		printf("  ��                              �� ");
		gotoxy(140, 49);
		printf("  ��           �ڷ� ����          �� ");
		gotoxy(140, 50);
		printf("  ��                              �� ");
		gotoxy(140, 51);
		printf("  ��                              �� ");
		gotoxy(140, 52);
		printf("  ���������������������������������������������������������������� ");
		if (GetAsyncKeyState(0x31) < 0) {
			unitnum = 1;
			break;
		}
		else if (GetAsyncKeyState(0x32) < 0) {
			unitnum = 2;
			break;
		}
		else if (GetAsyncKeyState(0x33) < 0) {
			unitnum = 3;
			break;
		}
		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k==0) {
			unitnum = 1;
			break;
		}
		else if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k==0) {
			unitnum = 2;
			break;
		}
		else if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k==0) {
			unitnum = 3;
			break;
		}
		else if (mouse_X >= 1146 && mouse_X <= 1390 && mouse_Y >= 740 && mouse_Y <= 840 && GetAsyncKeyState(VK_LBUTTON) < 0 && k==0) {
			eraseScreen();
			return difSel();
		}
		if (GetAsyncKeyState(VK_LBUTTON) < 0)	k = 1;
	}
	eraseScreen();
	gotoxy(60, 12);
	printf("  ������������������������������������������������������������������������������������������������������������������������ ");
	gotoxy(60, 13);
	printf("  ��                                                          �� ");
	gotoxy(60, 14);
	printf("  ��                                                          �� ");
	gotoxy(60, 15);
	printf("  ��                                                          �� ");
	gotoxy(60, 16);
	switch (unitnum) {
	case 1:
		printf("  ��               '���� 1��'�� ���õǾ����ϴ�.               �� ", difficulty);
		break;
	case 2:
		printf("  ��               '���� 2��'�� ���õǾ����ϴ�.               �� ", difficulty);
		break;
	case 3:
		printf("  ��               '���� 3��'�� ���õǾ����ϴ�.               �� ", difficulty);
		break;
	}
	gotoxy(60, 17);
	printf("  ��                                                          �� ");
	gotoxy(60, 18);
	printf("  ��                     ������ ���۵˴ϴ�.                   �� ");
	gotoxy(60, 19);
	printf("  ��                                                          �� ");
	gotoxy(60, 20);
	printf("  ������������������������������������������������������������������������������������������������������������������������ ");
	Sleep(1000);
	return;
}


void reset() {	//�����
	for (int i = 0; i < Unitcnt; i++) {
		Units[i].isUsed = 0;
		Enemy[i].isUsed = 0;
	}
	for (int i = 0; i < 640; i++) {
		for (int j = 0; j < 480; j++) {
			fullMap[i][j] = 0;
		}
	}
	eraseScreen();
	difSel();
	for (int i = 0; i < unitnum; i++) {
		createUnit(Medic, i * 100 + 150, i * 100 + 150);
	}
}

/* ���⼭���� �� ���� �Լ�*/
void alltargetAttack() {	//��濡�� �������� ����
	for (int i = 0; i < 1; i++) {
		createEnemy(Vulture, 1, Units[i].y, 8);
		createEnemy(Vulture, 640, Units[i].y, 24);
		createEnemy(Vulture, Units[i].x, 1, 16);
		createEnemy(Vulture, Units[i].x, 480, 0);
	}
}
void targetAttack() {	//�������� �ѹ������� ����
	for (int i = 0; i < 1; i++) {
		switch (rand() % 4) {
		case 0:
			createEnemy(Vulture, Units[i].x, 480, 0);
			break;
		case 1:
			createEnemy(Vulture, 1, Units[i].y, 8);
			break;
		case 2:
			createEnemy(Vulture, Units[i].x, 1, 16);
			break;
		case 3:
			createEnemy(Vulture, 640, Units[i].y, 24);
			break;
		}
	}
}
void sbombAttack() {	//���� �������� 3�� ���ð���
	int a = rand() % 4;
	for (int i = -20; i <= 20; i += 20) {
		switch (a) {
		case 0:
			createEnemy(Vulture, Units[0].x + i, 480, 0);
			break;
		case 1:
			createEnemy(Vulture, 1, Units[0].y + i, 8);
			break;
		case 2:
			createEnemy(Vulture, Units[0].x + i, 1, 16);
			break;
		case 3:
			createEnemy(Vulture, 640, Units[0].y + i, 24);
			break;
		}
	}
}
void bbombAttack() {	//���� �������� 5�� ���ð���
	int a = rand() % 4;
	for (int i = -40; i <= 40; i += 20) {
		switch (a) {
		case 0:
			createEnemy(Vulture, Units[0].x + i, 480, 0);
			break;
		case 1:
			createEnemy(Vulture, 1, Units[0].y + i, 8);
			break;
		case 2:
			createEnemy(Vulture, Units[0].x + i, 1, 16);
			break;
		case 3:
			createEnemy(Vulture, 640, Units[0].y + i, 24);
			break;
		}
	}
}


void leftAttack(int x) {	//���ʿ��� ���� ���� (���� 1, �������� 2 ��)
	if (x == 1) {
		for (int i = 0; i <= 480; i += 110 - difficulty * 10) {
			createEnemy(Vulture, 1, i, 8);
		}
	}
	if (x == 2) {
		for (int i = 40; i <= 480; i += 110 - difficulty * 10)
			createEnemy(Vulture, 1, i, 8);
	}
}

void upAttack(int x) {	//������ ���� ���� (���� 1, �������� 2 ��)
	if (x == 1) {
		for (int i = 0; i <= 640; i += 110 - difficulty * 10) {
			createEnemy(Vulture, i, 1, 16);
		}
	}
	if (x == 2) {
		for (int i = 40; i <= 640; i += 110 - difficulty * 10)
			createEnemy(Vulture, i, 1, 16);
	}
}

void randomAttack() {	// ��濡�� �������� �����
	for (int i = 0; i < difficulty + 1; i++) {
		switch (rand() % 4) {
		case 0:
			createEnemy(Vulture, rand() % 640 + 1, 480, 0);
			break;
		case 1:
			createEnemy(Vulture, 1, rand() % 480 + 1, 8);
			break;
		case 2:
			createEnemy(Vulture, rand() % 640 + 1, 1, 16);
			break;
		case 3:
			createEnemy(Vulture, 640, rand() % 480 + 1, 24);
			break;
		}
	}
}

void createHomingBomb() {	//������ź ����
	createEnemy(ScienceVessel, 1, 1, 0);
}

void homing() {	//���ֿ��� ����
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel) {
			if (Enemy[i].x < Units[0].x)	Enemy[i].x += difficulty + 1;
			else	Enemy[i].x -= (difficulty + 1);

			if (Enemy[i].y < Units[0].y)	Enemy[i].y += difficulty + 1;
			else	Enemy[i].y -= (difficulty + 1);

		}
	}
}

void homingAttack() {	//��ź���� ���� ������
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel && Enemy[i].isUsed == 1) {
			createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 0);
			createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 8);
			createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 16);
			createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 24);
		}
	}
}

void destroyHomingBomb() {	//��ź ��-��
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel && Enemy[i].isUsed == 1)	Enemy[i].isUsed = 0;
	}
}

void createBomb() {		//��ź ����
	createEnemy(ScienceVessel, 30, 30, 0);
}

void moveBomb() {	//��ź ������
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel) {
			if (Enemy[i].temp == 0) {
				Enemy[i].x += 2 + difficulty;
			}
			else if (Enemy[i].temp == 1) {
				Enemy[i].y += 2 + difficulty;
			}
			else if (Enemy[i].temp == 2) {
				Enemy[i].x -= 2 + difficulty;
			}
			else if (Enemy[i].temp == 3) {
				Enemy[i].y -= 2 + difficulty;
			}

			if (Enemy[i].x >= 600 && Enemy[i].y <= 30)	Enemy[i].temp = 1;
			else if (Enemy[i].y >= 440 && Enemy[i].x >= 600)	Enemy[i].temp = 2;
			else if (Enemy[i].x <= 30 && Enemy[i].y >= 440)	Enemy[i].temp = 3;
			else if (Enemy[i].y <= 30 && Enemy[i].x <= 30)	Enemy[i].temp = 0;
		}
	}
}

void bombAttack() {	//��ź ����
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel && Enemy[i].isUsed == 1) {
			if (Enemy[i].temp == 0)				createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 16);
			else if (Enemy[i].temp == 1)	createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 24);
			else if (Enemy[i].temp == 2)	createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 0);
			else if (Enemy[i].temp == 3)	createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 8);
		}
	}
}

void upFullAttack() {	//������ �����ϰ� ���� ����
	int a = rand() % 30 - 29;
	for (int i = 0; i <= 1260; i += 75) {
		createEnemy(Vulture, i + a, 1, 16);
	}
}

void leftFullAttack() {	//���ʿ��� �����ϰ� ���� ����
	int a = rand() % 30 - 29;
	for (int i = 0; i <= 480; i += 75) {
		createEnemy(Vulture, 1, i + a, 8);
	}
}
int Blast() {	//���� 3 ���� - �������� ���� ���� �� �� �ȿ� �ִ��� üũ
	for (int i = 0; i < Unitcnt; i++) {
		if (Units[i].isUsed == 1) {
			for (int y = -Units[i].sizeY / 2 - 1; y <= Units[i].sizeY / 2; y++) {
				for (int x = -Units[i].sizeX / 2 - 1; x <= Units[i].sizeX / 2; x++) {
					if (Units[i].x + x <= 0 || Units[i].y + y <= 0 || Units[i].x + x >= 4096 || Units[i].y + y >= 4096)	continue;
					if (x >= -Units[i].sizeX / 2 && x < Units[i].sizeX / 2 && y >= -Units[i].sizeY && y < Units[i].sizeY / 2)	continue;
					if (fullMap[Units[i].x + x][Units[i].y + y] != 5) {
						for (int a = 0; a < 640; a++) {
							for (int b=0; b<480; b++){
								if (fullMap[a][b] == 5)	fullMap[a][b] = 0;
							}
						}
						return 1;
					}
				}
			}
		}
	}
		
	return 0;
}

void startScreen() {	//����ȭ��
	gotoxy(70, 27);
	printf("VULTURE AVOID");
}

int gameClear() {

	gotoxy(60, 12);
	printf("  ������������������������������������������������������������������������������������������������������������������������ ");
	gotoxy(60, 13);
	printf("  ��                                                          �� ");
	gotoxy(60, 14);
	printf("  ��                      ���ϵ帳�ϴ�!                       �� ");
	gotoxy(60, 15);
	printf("  ��                                                          �� ");
	gotoxy(60, 16);
	printf("  ��                                                          �� ");
	gotoxy(60, 17);
	printf("  ��                 ������ Ŭ�����ϼ̽��ϴ�!                 �� ");
	gotoxy(60, 18);
	printf("  ��                                                          �� ");
	gotoxy(60, 19);
	printf("  ��                                                          �� ");
	gotoxy(60, 20);
	switch (difficulty) {
	case 1:
		printf("  ��                      ���̵� : ����                       �� ");
		break;
	case 2:
		printf("  ��                      ���̵� : ����                       �� ");
		break;
	case 3:
		printf("  ��                     ���̵� : �����                      �� ");
		break;
	}
	gotoxy(60, 21);
	printf("  ��                                                          �� ");
	gotoxy(60, 22);
	printf("  ��                                                          �� ");
	gotoxy(60, 23);
	switch (unitnum) {
	case 1:
		printf("  ��                     ���� ���� : 1��                      �� ");
		break;
	case 2:
		printf("  ��                     ���� ���� : 2��                      �� ");
		break;
	case 3:
		printf("  ��                     ���� ���� : 3��                      �� ");
		break;
	}
	gotoxy(60, 24);
	printf("  ��                                                          �� ");
	gotoxy(60, 25);
	printf("  ��                                                          �� ");
	gotoxy(60, 26);
	printf("  ��                                                          �� ");
	gotoxy(60, 27);
	printf("  ������������������������������������������������������������������������������������������������������������������������ ");
	k = 1;
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0)	k = 0;

		if (mouse_X >= 240 && mouse_X <= 710 && mouse_Y >= 570 && mouse_Y <= 690)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(27, 35);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(27, 36);
		printf("  ��                                                          �� ");
		gotoxy(27, 37);
		printf("  ��                                                          �� ");
		gotoxy(27, 38);
		printf("  ��                                                          �� ");
		gotoxy(27, 39);
		printf("  ��                       1. �ٽ� �ϱ�                       �� ");
		gotoxy(27, 40);
		printf("  ��                                                          �� ");
		gotoxy(27, 41);
		printf("  ��                                                          �� ");
		gotoxy(27, 42);
		printf("  ��                                                          �� ");
		gotoxy(27, 43);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		if (mouse_X >= 770 && mouse_X <= 1240 && mouse_Y >= 570 && mouse_Y <= 690)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(93, 35);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(93, 36);
		printf("  ��                                                          �� ");
		gotoxy(93, 37);
		printf("  ��                                                          �� ");
		gotoxy(93, 38);
		printf("  ��                                                          �� ");
		gotoxy(93, 39);
		printf("  ��                       2. ���� ����                       �� ");
		gotoxy(93, 40);
		printf("  ��                                                          �� ");
		gotoxy(93, 41);
		printf("  ��                                                          �� ");
		gotoxy(93, 42);
		printf("  ��                                                          �� ");
		gotoxy(93, 43);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(0, 0);

		if (GetAsyncKeyState(0x31) < 0 || (mouse_X >= 240 && mouse_X <= 710 && mouse_Y >= 570 && mouse_Y <= 690 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0)) {
			return 1;
		}

		else if (GetAsyncKeyState(0x32) < 0 || (mouse_X >= 770 && mouse_X <= 1240 && mouse_Y >= 570 && mouse_Y <= 690 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0)) {
			return 0;
		}

		if (GetAsyncKeyState(VK_LBUTTON) < 0)	k = 1;
	}
}

void intro() {
	int k = 0;
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0)	k = 0;
		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(0, 17);
		initCursor();
		printf("  ������������������������������������������������������������������������������������������������������������������������ \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                        1.���� �����ϱ�                   �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                       ������ �����մϴ�.                 �� \n");
		printf("  ��                ���̵� ���� ȭ������ �Ѿ�ϴ�.          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ��                                                          �� \n");
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");

		if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(63, 17);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(63, 18);
		printf("  ��                                                          �� ");
		gotoxy(63, 19);
		printf("  ��                                                          �� ");
		gotoxy(63, 20);
		printf("  ��                                                          �� ");
		gotoxy(63, 21);
		printf("  ��                                                          �� ");
		gotoxy(63, 22);
		printf("  ��                                                          �� ");
		gotoxy(63, 23);
		printf("  ��                                                          �� ");
		gotoxy(63, 24);
		printf("  ��                                                          �� ");
		gotoxy(63, 25);
		printf("  ��                         2. ���� ����                   �� ");
		gotoxy(63, 26);
		printf("  ��                                                          �� ");
		gotoxy(63, 27);
		printf("  ��                                                          �� ");
		gotoxy(63, 28);
		printf("  ��                                                          �� ");
		gotoxy(63, 29);
		printf("  ��                                                          �� ");
		gotoxy(63, 30);
		printf("  ��                                                          �� ");
		gotoxy(63, 31);
		printf("  ��                      ������ ������ ���ϴ�.               �� ");
		gotoxy(63, 32);
		printf("  ��                                                          �� ");
		gotoxy(63, 33);
		printf("  ��                                                          �� ");
		gotoxy(63, 34);
		printf("  ��                                                          �� ");
		gotoxy(63, 35);
		printf("  ��                                                          �� ");
		gotoxy(63, 36);
		printf("  ��                                                          �� ");
		gotoxy(63, 37);
		printf("  ��                                                          �� ");
		gotoxy(63, 38);
		printf("  ��                                                          �� ");
		gotoxy(63, 39);
		printf("  ��                                                          �� ");
		gotoxy(63, 40);
		printf("  ��                                                          �� ");
		gotoxy(63, 41);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");


		if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(126, 17);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");
		gotoxy(126, 18);
		printf("  ��                                                          �� ");
		gotoxy(126, 19);
		printf("  ��                                                          �� ");
		gotoxy(126, 20);
		printf("  ��                                                          �� ");
		gotoxy(126, 21);
		printf("  ��                                                          �� ");
		gotoxy(126, 22);
		printf("  ��                                                          �� ");
		gotoxy(126, 23);
		printf("  ��                                                          �� ");
		gotoxy(126, 24);
		printf("  ��                                                          �� ");
		gotoxy(126, 25);
		printf("  ��                         3. ���� ����                     �� ");
		gotoxy(126, 26);
		printf("  ��                                                          �� ");
		gotoxy(126, 27);
		printf("  ��                                                          �� ");
		gotoxy(126, 28);
		printf("  ��                                                          �� ");
		gotoxy(126, 29);
		printf("  ��                                                          �� ");
		gotoxy(126, 30);
		printf("  ��                                                          �� ");
		gotoxy(126, 31);
		printf("  ��                       ������ �����մϴ�.                 �� ");
		gotoxy(126, 32);
		printf("  ��                                                          �� ");
		gotoxy(126, 33);
		printf("  ��                                                          �� ");
		gotoxy(126, 34);
		printf("  ��                                                          �� ");
		gotoxy(126, 35);
		printf("  ��                                                          �� ");
		gotoxy(126, 36);
		printf("  ��                                                          �� ");
		gotoxy(126, 37);
		printf("  ��                                                          �� ");
		gotoxy(126, 38);
		printf("  ��                                                          �� ");
		gotoxy(126, 39);
		printf("  ��                                                          �� ");
		gotoxy(126, 40);
		printf("  ��                                                          �� ");
		gotoxy(126, 41);
		printf("  ������������������������������������������������������������������������������������������������������������������������ ");


		if (GetAsyncKeyState(0x31) < 0 || (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0) && k == 0) {
			eraseScreen();
			gotoxy(60, 12);
			printf("  ������������������������������������������������������������������������������������������������������������������������ ");
			gotoxy(60, 13);
			printf("  ��                                                          �� ");
			gotoxy(60, 14);
			printf("  ��                                                          �� ");
			gotoxy(60, 15);
			printf("  ��                                                          �� ");
			gotoxy(60, 16);
			printf("  ��               ���̵� ���� ȭ������ �Ѿ�ϴ�.           �� ");
			gotoxy(60, 17);
			printf("  ��                                                          �� ");
			gotoxy(60, 18);
			printf("  ��                                                          �� ");
			gotoxy(60, 19);
			printf("  ��                                                          �� ");
			gotoxy(60, 20);
			printf("  ������������������������������������������������������������������������������������������������������������������������ ");
			Sleep(1000);
			eraseScreen();
			break;
		}
		else if (GetAsyncKeyState(0x32) < 0 || (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0) && k == 0) {
			eraseScreen();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			gotoxy(30, 6);
			printf("  �������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������� ");
			gotoxy(30, 7);
			printf("  ��                                                                                                              �� ");
			gotoxy(30, 8);
			printf("  ��                         �� ������ 1998�⿡ ����� Starcraft�� ������ũ�� �����Դϴ�.                         �� ");
			gotoxy(30, 9);
			printf("  ��                      �� ������ ���ΰ��� Starcraft�� ���� ĳ���� �� �ϳ��� Medic�Դϴ�.                       �� ");
			gotoxy(30, 10);
			printf("  ��                         Medic�� ���� ���⿡�� ���ƿ��� ���� Vulture�� ���ؾ� �մϴ�.                         �� ");
			gotoxy(30, 11);
			printf("  ��                             �� ������������ Vulture �� �ٸ� �������� ���ƿɴϴ�.                             �� ");
			gotoxy(30, 12);
			printf("  ��                     ���ƿ��� Vulture�� ������ ���ϸ� Medic�� �װ� ������ ������ �˴ϴ�.                      �� ");
			gotoxy(30, 13);
			printf("  ��                                                                                                              �� ");
			gotoxy(30, 14);
			printf("  ��                        �޴������� ������ ���̵��� Medic�� ���� ������ �� �ֽ��ϴ�.                           �� ");
			gotoxy(30, 15);
			printf("  ��            ���̵��� �� 3�ܰ�� ������ ���̵��� ���� ���ƿ��� Vulture�� ���� �ӵ��� �ٸ��ϴ�.                 �� ");
			gotoxy(30, 16);
			printf("  ��    Medic�� �ִ� 3����� �÷��� �� �� ������ ���콺 �巡�� �Ǵ� Ű������ 1,2,3 ��ư���� ������ �� �ֽ��ϴ�.   �� ");
			gotoxy(30, 17);
			printf("  ��             Medic�� �����ϰ� �̵��ϰ� ���� ��ġ�� ���콺 ��Ŭ���� �ϸ� �޵��� �װ����� �̵��մϴ�.           �� ");
			gotoxy(30, 18);
			printf("  ��            Medic���� ���� ���ÿ� �÷��� �� ���� Medic���� ��Ƶ� �޵��� �װ�, ������ ���� ���ϴ�.            �� ");
			gotoxy(30, 19);
			printf("  ��                                                                                                              �� ");
			gotoxy(30, 20);
			printf("  ��                Vulture�� �� ���������� Ŭ���� �� ������ ������ �ٸ� �ɷ��� ������ �����ɴϴ�.                �� ");
			gotoxy(30, 21);
			printf("  ��                    Medic �� ��� Vulture �� ������ ������ ���ϸ� ������ Ŭ�����ϰ� �˴ϴ�.                   �� ");
			gotoxy(30, 22);
			printf("  ��                                                                                                              �� ");
			gotoxy(30, 23);
			printf("  ��                           �������� ������ ���������� ������ ������ �����ϴ�.                                 �� ");
			gotoxy(30, 24);
			printf("  ��                           - �������� 1 : 0 ~ 2000                                                            �� ");
			gotoxy(30, 25);
			printf("  ��                           - ���� 1   : 2000 ~ 4000 (BattleCruiser)                                           �� ");
			gotoxy(30, 26);
			printf("  ��                           - �������� 2 : 4000 ~ 6000                                                         �� ");
			gotoxy(30, 27);
			printf("  ��                           - ���� 2   : 6000 ~ 8000 (ScienceVessel)                                           �� ");
			gotoxy(30, 28);
			printf("  ��                           - �������� 3 : 8000 ~ 10000                                                        �� ");
			gotoxy(30, 29);
			printf("  ��                           - ���� 3   : 10000 ~ 17000 (Ghost)                                                 �� ");
			gotoxy(30, 30);
			printf("  ��                                                                                                              �� ");
			gotoxy(30, 31);
			printf("  ��                                                                                                              �� ");
			gotoxy(30, 32);
			printf("  ��                                                                                                              �� ");
			gotoxy(30, 33);
			printf("  �������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������� ");

			while (1) {
				if (GetAsyncKeyState(VK_LBUTTON) >= 0)	k = 0;
				if (mouse_X >= 1146 && mouse_X <= 1390 && mouse_Y >= 740 && mouse_Y <= 840)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				gotoxy(140, 46);
				printf("  ���������������������������������������������������������������� ");
				gotoxy(140, 47);
				printf("  ��                              �� ");
				gotoxy(140, 48);
				printf("  ��                              �� ");
				gotoxy(140, 49);
				printf("  ��           �ڷ� ����          �� ");
				gotoxy(140, 50);
				printf("  ��                              �� ");
				gotoxy(140, 51);
				printf("  ��                              �� ");
				gotoxy(140, 52);
				printf("  ���������������������������������������������������������������� ");
				if (mouse_X >= 1146 && mouse_X <= 1390 && mouse_Y >= 740 && mouse_Y <= 840 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
					eraseScreen();
					return intro();
				}
				if (GetAsyncKeyState(VK_LBUTTON) < 0)	k = 1;
			}
			break;
		}
		else if (GetAsyncKeyState(0x33) < 0 || (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k==0)) {
			gameover = 1;
			break;
		}
		if (GetAsyncKeyState(VK_LBUTTON) < 0)	k = 1;
	}
}
int main() {
	srand(time(0));
	HWND hWnd = getConsoleWindowHandle();
	dpi = GetDPI(hWnd);
	paint(hWnd);
	CreateThread(NULL, 8 * 1024 * 1024, mouseInput, &hWnd, NULL, NULL);
	CreateThread(NULL, 8 * 1024 * 1024, Soundmain, NULL, NULL, NULL);
	fullscreenConsole();
	imgLoad();
	mouseMode = arrow;
	HANDLE CIN = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
	intro();
	if (gameover == 1)	return 0;
	difSel();
	sounds[4].isused = 1;	//���� ������ �Ҹ�
	for (int i = 0; i < unitnum; i++) {
		createUnit(Medic, i * 100 + 150, i * 100 + 150);	//���� ������ŭ ����
	}
	score = 0;
	
	while (1)
	{
		
		gotoxy(0, 0);
		long long oldTime = GetTickCount64();
		score++;
		// �������� �� ���� ���� ����	������ ���� �ּ��� ��...����
		if (score >=0 && score < 2000) {	//stage 1

			if (rand() % 1000 <= 3 * difficulty + 1) {
				createEnemy(Vulture, 1, rand() % 480, 8);
			}
			else if (rand() % 1000 <= 3 * difficulty * 2 + 1) {
				createEnemy(Vulture, rand() % 500, 1, 16);
			}
			else if (rand() % 1000 <= 3 * difficulty * 3 + 1) {
				createEnemy(Vulture, 640, rand() % 480, 24);
			}
			else if (rand() % 1000 <= 3 * difficulty * 4 + 1) {
				createEnemy(Vulture, rand() % 500, 480, 0);
			}

			if (score % (600 - difficulty * 40) == 0 && score >= 1000)	bbombAttack();

			if (score % (110 - difficulty * 10) == 0)	targetAttack();

			if (score % (450 - difficulty * 40) == 0) {
				if (rand() % 2)	leftAttack(1);
				else	upAttack(2);
			}
		}
		if (score >= 2200 && score < 4000) {	//boss1
			if (score == 2201)	createEnemy(BattleCruiser, 320, 1, 16), sounds[1].isused = 1;
			if (score % (150 - 15 * difficulty) == 0 && score < 3000)	leftAttack(1);
			if (score % (150 - 15 * difficulty) == (150 - 15 * difficulty) / 2 && score < 3000)	leftAttack(2);
			if (score % 1000 == 750)	sbombAttack();
			else if (score % 1000 == 500)	sbombAttack();
			else if (score % 1000 == 250)	alltargetAttack();
			if (score % (110 - difficulty * 10) == 0)	targetAttack();
			if (score % (160 - 13 * difficulty) == 0 && score > 3000)	upAttack(1);
			if (score % (160 - 13 * difficulty) == (150 - 15 * difficulty) / 2 && score > 3000)	upAttack(2);
		}
		if (score >= 4300 && score < 6000) {	//stage2
			if (score > 50) {
				if (rand() % 1200 <= 2 * difficulty + 1) {
					createEnemy(Vulture, 1, rand() % 480, 8);
				}
				else if (rand() % 1200 <= 3 * difficulty * 2) {
					createEnemy(Vulture, rand() % 500, 1, 16);
				}
				else if (rand() % 1200 <= 3 * difficulty * 3) {
					createEnemy(Vulture, 640, rand() % 480, 24);
				}
				else if (rand() % 1200 <= 3 * difficulty * 4) {
					createEnemy(Vulture, rand() % 500, 480, 0);
				}
			}

			if (score % (700 - difficulty * 30) == 0) alltargetAttack();

			if (score % (500 - difficulty * 35) == 0 && score < 1000)	sbombAttack();

			if (score % (500 - difficulty * 35) == 0 && score >= 1000)	bbombAttack();


			if (score % (130 - difficulty * 10) == 0)	targetAttack();



		}
		if (score >= 6200 && score < 8000) {	//Boss2
			if (score == 6200)	sounds[6].isused = 1;
			if (score % 300 <= 100 && score % 35 == 0)	alltargetAttack();
			if (score % 250 == 0)	homingAttack();
			if (score % 300 == 0 && score % 600 != 0)	leftAttack(1);
			if (score % 600 == 0)	upAttack(1);
			if (score == 6200 || score == 7000)	createHomingBomb();
			if (score % 5 == 0)	homing();
		}
		if (score == 8000) {
			destroyHomingBomb();
		}

		if (score >= 8200 && score < 10000) {	//Stage 3
			if (score % 65 - 5 * difficulty == 0)	randomAttack();
			if (score % 67 - 5 * difficulty == 0)	targetAttack();
			if (score == 8200)	createBomb();
			if (difficulty >= 2 && score == 8400)	createBomb();
			if (score == 8800)	createBomb();
			if (difficulty == 3 && score == 8600)	createBomb();
			moveBomb();
			if (score % 35 - difficulty * 5 == 0)	bombAttack();

		}
		if (score == 10000)	destroyHomingBomb();
		if (score >= 10200 && score < 12000) {	//Boss3 - 1
			if (score == 10200)	sounds[2].isused = 1;
			if (score == 10200)	createEnemy(Ghost, 320, 30, 16);
			if (score % 100 == 0 && score % 200 != 0)	upFullAttack();
			if (score % 200 == 0)	leftFullAttack();
			if (score % 115 == 0)	alltargetAttack();
			if (score % 250 == 0)	sbombAttack();
		}

		if (score >= 12000 && score < 15000) {	//Boss3 - 2
			if (score % 500 == 0)	blast = 1, blastx = rand() % 300 + 200, blasty = rand() % 240 + 120, sounds[0].isused=1;
			if (score % 500 == 500-100*difficulty) {
				blast = 0;
				sounds[5].isused = 1;
				if (Blast() == 1)	gameover = 1;
			}

			if (score < 12500) {
				if (score % 40-5*difficulty == 0)	randomAttack();
			}

			if (score >= 12500 && score < 13000) {
				if (score % 35 - 5 * difficulty == 0)	randomAttack();
			}
			if (score >= 13000 && score < 13500) {
				if (score % 30 - 5 * difficulty == 0)	randomAttack();
			}
			
			if (score >= 13500 && score < 14000) {
				if (score % 40 - 5 * difficulty == 0)	randomAttack();
				if (score % 100 - 10 * difficulty == 0)	targetAttack();
				if (score % 172 - 15 * difficulty == 0)	alltargetAttack();
			}
			if (score >= 14000 && score < 15000) {
				if (score % 110 - 17 * difficulty == 0)	randomAttack();
				if (score % 230-7*difficulty == 0)	leftAttack(1), upAttack(1);
				if (score % 276 - 7 * difficulty == 0)	leftAttack(0), upAttack(0);
				if (score % 97 - 8 * difficulty == 0)	alltargetAttack();
			}
		}

		if (score >= 15000 && score < 17000) {	//Boss3-3
			if (score % 500 == 0)	blast = 1, blastx = rand() % 300 + 200, blasty = rand() % 240 + 120, sounds[0].isused = 1;
			if (score % 500 == 500 - 100 * difficulty) {
				blast = 0;
				sounds[5].isused = 1;
				if (Blast() == 1)	gameover = 1;
			}
			if (score == 15000 || score == 16000)	createHomingBomb();
			if (score % 7 == 0)	homing();
			if (score % 150 - 10 * difficulty == 0)	homingAttack();
			if (score % 73 - 5*difficulty == 0)	bombAttack();
			if (score % 86 - difficulty * 5 == 0)	randomAttack();
			if (score % 126 == 0)	alltargetAttack();
		}

		if (score == 17500) {
			eraseScreen();
			if (gameClear() == 1) {
				score = -200;
			}
			else
			{
				return 0;
			}

		}
		//���� �浹 üũ �� ���� ����/�����
		for (int i = 0; i < unitnum; i++) {
			if (collisionCheck(i) == 1 || gameover==1) {
				sounds[3].isused = 1;
				for (int j = 222;j < 230;j++) {
					for (int i = 0; i < unitnum; i++) {
						Units[i].Death = j;
					}
					paintGroundUnits(img);
					Sleep(100);
				}
				eraseScreen();
				if (gameContinue() == 1) {
					reset();
					score = 0;
					sounds[4].isused = 1;
					continue;
				}
				return;
			}
		}
		if (score == 0)	continue;
		 
		LinearMove();	// ���� �̵�


		initxy();	//�ȸ������ ���� xy �Űܹ�����
		initCursor();	//Ŀ�� �����̱� ���ֱ�
		inMapCheck();	//�� ���� �Լ�
		processInput();		//��ǲ ���μ���

		//��üũ
		paint(hWnd);
		long long nowTime = GetTickCount64();
		if (nowTime - oldTime <= 16)
			Sleep(16 - (nowTime - oldTime));
	}
}