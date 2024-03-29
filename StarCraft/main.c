#include "header.h"
#include "terran.h"
#include "UI.h"
#include "Sound.h"




DWORD __stdcall Soundmain(void* p);



void processInput()	// input 薯橫
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

	if (GetAsyncKeyState(VK_LBUTTON) < 0) { // 豭贗葛
		mouseMode = drag;
		click++;
		if (click == 1) {	//萄楚斜 衛濛
			start_x = GDImouse_X + scroll_X;
			start_y = GDImouse_Y + scroll_Y;
		}
	}
	else if (abs(start_x - GDImouse_X - scroll_X) < 15 && abs(start_y - GDImouse_Y - scroll_Y) < 15 && click > 0) {	//贗葛
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
	else if (click > 0)	//萄楚斜 部
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

	if (GetAsyncKeyState(VK_RBUTTON) < 0) {	//辦贗葛
		mouseMode = targg;
		int prev = -1, destx = 0, desty = 0;
		int prevnum = 0;
		for (int i = 0; i < Unitcnt; i++) {
			if (Units[i].selected == 1 && Units[i].temp == 0) {
				Units[i].temp = 1;	//缽援腦晦 寞雖辨
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

DWORD __stdcall mouseInput(void* param)	//葆辦蝶 殮溘
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


int createUnit(int name, int x, int y) {	//嶸棉 儅撩
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

int createEnemy(int name, int x, int y, int dir) {	//瞳 儅撩
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

void initxy() {	//儅撩寰脹嶸棉 xy高 蟾晦��
	for (int i = 0; i < Unitcnt; i++) {
		if (Units[i].isUsed == 0) {
			Units[i].x = -9999;
			Units[i].y = -9999;
		}
	}
}

int collisionCheck(int i) {	//醱給 羹觼

	for (int y = -Units[i].sizeY / 2 - 1; y <= Units[i].sizeY / 2; y++) {
		for (int x = -Units[i].sizeX / 2 - 1; x <= Units[i].sizeX / 2; x++) {
			if (Units[i].x + x <= 0 || Units[i].y + y <= 0 || Units[i].x + x >= 4096 || Units[i].y + y >= 4096)	continue;
			if (x >= -Units[i].sizeX / 2 && x < Units[i].sizeX / 2 && y >= -Units[i].sizeY && y < Units[i].sizeY / 2)	continue;
			if (fullMap[Units[i].x + x][Units[i].y + y] == 1 || fullMap[Units[i].x + x][Units[i].y + y] == 4) {
				return 1;
			}
		}
	}
	return 0;
}

int checkDirection(int x, int y, int destx, int desty) {	//寞щ 羹觼
	double g = (double)(desty - y) / (double)(destx - x); // g : 晦選晦
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

void eraseScreen() {	//蝶觼萼 雖辦晦
	gotoxy(0, 0);
	for (int i = 0; i < 1010; i++)
		printf("             ");
	return;
}


void LinearMove() {	//遺霜歜
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

int initLinearMove(int start_x, int start_y, int end_x, int end_y, int idx) {	//望瓊晦 (霜摹)
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


int gameContinue() {	//營衛濛 罹睡 �挫�


	gotoxy(60, 12);
	printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
	gotoxy(60, 13);
	printf("  弛                                                          弛 ");
	gotoxy(60, 14);
	printf("  弛                                                          弛 ");
	gotoxy(60, 15);
	printf("  弛                                                          弛 ");
	gotoxy(60, 16);
	printf("  弛                        啪歜 螃幗                         弛 ");
	gotoxy(60, 17);
	printf("  弛                                                          弛 ");
	gotoxy(60, 18);
	printf("  弛                                                          弛 ");
	gotoxy(60, 19);
	printf("  弛                 棻衛 Ы溯檜ж衛啊蝗棲梱?                 弛 ");
	gotoxy(60, 20);
	printf("  弛                                                          弛 ");
	gotoxy(60, 21);
	printf("  弛                                                          弛 ");
	gotoxy(60, 22);
	printf("  弛                                                          弛 ");
	gotoxy(60, 23);
	printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
	k = 1;
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0)	k = 0;

		if (mouse_X >= 240 && mouse_X <= 710 && mouse_Y >= 570 && mouse_Y <= 690)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(27, 35);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(27, 36);
		printf("  弛                                                          弛 ");
		gotoxy(27, 37);
		printf("  弛                                                          弛 ");
		gotoxy(27, 38);
		printf("  弛                                                          弛 ");
		gotoxy(27, 39);
		printf("  弛                       1. 棻衛 ж晦                       弛 ");
		gotoxy(27, 40);
		printf("  弛                                                          弛 ");
		gotoxy(27, 41);
		printf("  弛                                                          弛 ");
		gotoxy(27, 42);
		printf("  弛                                                          弛 ");
		gotoxy(27, 43);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
		if (mouse_X >= 770 && mouse_X <= 1240 && mouse_Y >= 570 && mouse_Y <= 690)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(93, 35);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(93, 36);
		printf("  弛                                                          弛 ");
		gotoxy(93, 37);
		printf("  弛                                                          弛 ");
		gotoxy(93, 38);
		printf("  弛                                                          弛 ");
		gotoxy(93, 39);
		printf("  弛                       2. 啪歜 謙猿                       弛 ");
		gotoxy(93, 40);
		printf("  弛                                                          弛 ");
		gotoxy(93, 41);
		printf("  弛                                                          弛 ");
		gotoxy(93, 42);
		printf("  弛                                                          弛 ");
		gotoxy(93, 43);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
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


void difSel() {	//陪檜紫 撲薑
	int k = 1;
	Sleep(200);
	initCursor();
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0) k = 0;
		gotoxy(85, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("陪檜紫蒂 摹鷗ж撮蹂.\n");
		gotoxy(0, 17);
		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                        1.蓮遺                            弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                    - 弊藥 樓紫 替葡                      弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                    - 蝶ア榆 瞳擠                         弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                    - 爾蝶 ぬ欐 蓮遺                      弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛             陛濰 蓮遴 陪檜紫. 遺霜檜朝 寞衝檜            弛 \n");
		printf("  弛             櫛熨ж雖 彊剪釭 Яж晦盟曖 啪歜擊            弛 \n");
		printf("  弛             澀 跤ж朝 蟾褕濠 瞪辨 陪檜紫.                弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");

		if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(63, 17);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(63, 18);
		printf("  弛                                                          弛 ");
		gotoxy(63, 19);
		printf("  弛                                                          弛 ");
		gotoxy(63, 20);
		printf("  弛                                                          弛 ");
		gotoxy(63, 21);
		printf("  弛                                                          弛 ");
		gotoxy(63, 22);
		printf("  弛                                                          弛 ");
		gotoxy(63, 23);
		printf("  弛                                                          弛 ");
		gotoxy(63, 24);
		printf("  弛                                                          弛 ");
		gotoxy(63, 25);
		printf("  弛                         2. 爾鱔                          弛 ");
		gotoxy(63, 26);
		printf("  弛                                                          弛 ");
		gotoxy(63, 27);
		printf("  弛                                                          弛 ");
		gotoxy(63, 28);
		printf("  弛                     - 弊藥 樓紫 爾鱔                     弛 ");
		gotoxy(63, 29);
		printf("  弛                                                          弛 ");
		gotoxy(63, 30);
		printf("  弛                     - 蝶ア榆 爾鱔                        弛 ");
		gotoxy(63, 31);
		printf("  弛                                                          弛 ");
		gotoxy(63, 32);
		printf("  弛                     - 爾蝶 陪檜紫 爾鱔                   弛 ");
		gotoxy(63, 33);
		printf("  弛                                                          弛 ");
		gotoxy(63, 34);
		printf("  弛                                                          弛 ");
		gotoxy(63, 35);
		printf("  弛                                                          弛 ");
		gotoxy(63, 36);
		printf("  弛                爾鱔 陪檜紫. 蓮遺 陪檜紫蒂                弛 ");
		gotoxy(63, 37);
		printf("  弛                蔣啪 產渦塭紫 褻旎擎 ��菟 熱              弛 ");
		gotoxy(63, 38);
		printf("  弛                氈朝 陪檜紫.                              弛 ");
		gotoxy(63, 39);
		printf("  弛                                                          弛 ");
		gotoxy(63, 40);
		printf("  弛                                                          弛 ");
		gotoxy(63, 41);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");


		if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(126, 17);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(126, 18);
		printf("  弛                                                          弛 ");
		gotoxy(126, 19);
		printf("  弛                                                          弛 ");
		gotoxy(126, 20);
		printf("  弛                                                          弛 ");
		gotoxy(126, 21);
		printf("  弛                                                          弛 ");
		gotoxy(126, 22);
		printf("  弛                                                          弛 ");
		gotoxy(126, 23);
		printf("  弛                                                          弛 ");
		gotoxy(126, 24);
		printf("  弛                                                          弛 ");
		gotoxy(126, 25);
		printf("  弛                         3. 橫溥遺                        弛 ");
		gotoxy(126, 26);
		printf("  弛                                                          弛 ");
		gotoxy(126, 27);
		printf("  弛                                                          弛 ");
		gotoxy(126, 28);
		printf("  弛                     - 弊藥 樓紫 緒葷                     弛 ");
		gotoxy(126, 29);
		printf("  弛                                                          弛 ");
		gotoxy(126, 30);
		printf("  弛                     - 蝶ア榆 號擠                        弛 ");
		gotoxy(126, 31);
		printf("  弛                                                          弛 ");
		gotoxy(126, 32);
		printf("  弛                     - 爾蝶 陪檜紫 橫溥遺                 弛 ");
		gotoxy(126, 33);
		printf("  弛                                                          弛 ");
		gotoxy(126, 34);
		printf("  弛                                                          弛 ");
		gotoxy(126, 35);
		printf("  弛                                                          弛 ");
		gotoxy(126, 36);
		printf("  弛                斜傖 梟晦朝 橫溥遴 陪檜紫.                弛 ");
		gotoxy(126, 37);
		printf("  弛                ぬ欐縑 渠и 堪擎 檜п諦                   弛 ");
		gotoxy(126, 38);
		printf("  弛                �К〥� 鐘お煤檜 氈橫撿                    弛 ");
		gotoxy(126, 39);
		printf("  弛                贗葬橫 陛棟и 陪檜紫.                     弛 ");
		gotoxy(126, 40);
		printf("  弛                                                          弛 ");
		gotoxy(126, 41);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");

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


		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
			difficulty = 1;
			break;
		}
		else if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
			difficulty = 2;
			break;
		}
		else if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
			difficulty = 3;
			break;
		}

		if (GetAsyncKeyState(VK_LBUTTON) < 0) k = 1;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	eraseScreen();

	gotoxy(60, 12);
	printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
	gotoxy(60, 13);
	printf("  弛                                                          弛 ");
	gotoxy(60, 14);
	printf("  弛                                                          弛 ");
	gotoxy(60, 15);
	printf("  弛                                                          弛 ");
	gotoxy(60, 16);
	switch (difficulty) {
	case 1:
		printf("  弛               '蓮遺' 陪檜紫陛 摹鷗腎歷蝗棲棻.            弛 ", difficulty);
		break;
	case 2:
		printf("  弛               '爾鱔' 陪檜紫陛 摹鷗腎歷蝗棲棻.            弛 ", difficulty);
		break;
	case 3:
		printf("  弛               '橫溥遺' 陪檜紫陛 摹鷗腎歷蝗棲棻.          弛 ", difficulty);
		break;
	}
	gotoxy(60, 17);
	printf("  弛                                                          弛 ");
	gotoxy(60, 18);
	printf("  弛                                                          弛 ");
	gotoxy(60, 19);
	printf("  弛                                                          弛 ");
	gotoxy(60, 20);
	printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
	Sleep(1000);
	eraseScreen();
	initCursor();
	k = 1;
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0) k = 0;
		gotoxy(85, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("陪檜紫蒂 摹鷗ж撮蹂.\n");
		gotoxy(0, 17);
		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                        1.嶸棉 1晦                        弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                陛濰 橾奩瞳檣 Ы溯檜 寞衝.                弛 \n");
		printf("  弛                欽牖и Яж晦 啪歜擊 闌晦堅               弛 \n");
		printf("  弛                談棻賊, 檜 寞衝擊 摹鷗                    弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");

		if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(63, 17);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(63, 18);
		printf("  弛                                                          弛 ");
		gotoxy(63, 19);
		printf("  弛                                                          弛 ");
		gotoxy(63, 20);
		printf("  弛                                                          弛 ");
		gotoxy(63, 21);
		printf("  弛                                                          弛 ");
		gotoxy(63, 22);
		printf("  弛                                                          弛 ");
		gotoxy(63, 23);
		printf("  弛                                                          弛 ");
		gotoxy(63, 24);
		printf("  弛                                                          弛 ");
		gotoxy(63, 25);
		printf("  弛                         2. 嶸棉 2晦                      弛 ");
		gotoxy(63, 26);
		printf("  弛                                                          弛 ");
		gotoxy(63, 27);
		printf("  弛                                                          弛 ");
		gotoxy(63, 28);
		printf("  弛                                                          弛 ");
		gotoxy(63, 29);
		printf("  弛                                                          弛 ");
		gotoxy(63, 30);
		printf("  弛                嶸棉 2晦煎 弊藥蒂 Яж朝 寞衝.            弛 ");
		gotoxy(63, 31);
		printf("  弛                濠褐曖 詩じ鷓蝶韁 棟溘檜 謠剪釭           弛 ");
		gotoxy(63, 32);
		printf("  弛                嶸棉 1晦煎 贗葬橫朝 傘鼠 蔣棻堅           弛 ");
		gotoxy(63, 33);
		printf("  弛                儅陝腆 陽 Ы溯檜ж賊 腆 陪檜紫.           弛 ");
		gotoxy(63, 34);
		printf("  弛                                                          弛 ");
		gotoxy(63, 35);
		printf("  弛                                                          弛 ");
		gotoxy(63, 36);
		printf("  弛                                                          弛 ");
		gotoxy(63, 37);
		printf("  弛                                                          弛 ");
		gotoxy(63, 38);
		printf("  弛                                                          弛 ");
		gotoxy(63, 39);
		printf("  弛                                                          弛 ");
		gotoxy(63, 40);
		printf("  弛                                                          弛 ");
		gotoxy(63, 41);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");


		if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(126, 17);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(126, 18);
		printf("  弛                                                          弛 ");
		gotoxy(126, 19);
		printf("  弛                                                          弛 ");
		gotoxy(126, 20);
		printf("  弛                                                          弛 ");
		gotoxy(126, 21);
		printf("  弛                                                          弛 ");
		gotoxy(126, 22);
		printf("  弛                                                          弛 ");
		gotoxy(126, 23);
		printf("  弛                                                          弛 ");
		gotoxy(126, 24);
		printf("  弛                                                          弛 ");
		gotoxy(126, 25);
		printf("  弛                         3. 嶸棉 3晦                      弛 ");
		gotoxy(126, 26);
		printf("  弛                                                          弛 ");
		gotoxy(126, 27);
		printf("  弛                                                          弛 ");
		gotoxy(126, 28);
		printf("  弛                                                          弛 ");
		gotoxy(126, 29);
		printf("  弛                                                          弛 ");
		gotoxy(126, 30);
		printf("  弛                ж萄囀橫 陪檜紫. 嬴鼠葬 澀п紫            弛 ");
		gotoxy(126, 31);
		printf("  弛                檜 薑紫曖 陪檜紫蒂 贗葬橫 й 熱           弛 ");
		gotoxy(126, 32);
		printf("  弛                氈擊雖 曖褕腎晦 衛濛ж朝 陪檜紫.          弛 ");
		gotoxy(126, 33);
		printf("  弛                贗葬橫ж賊, 霞雖ж啪 Щ煎啪檜該           弛 ");
		gotoxy(126, 34);
		printf("  弛                瞪щ擊 堅團п 瑭紫 腆 薑紫.               弛 ");
		gotoxy(126, 35);
		printf("  弛                                                          弛 ");
		gotoxy(126, 36);
		printf("  弛                                                          弛 ");
		gotoxy(126, 37);
		printf("  弛                                                          弛 ");
		gotoxy(126, 38);
		printf("  弛                                                          弛 ");
		gotoxy(126, 39);
		printf("  弛                                                          弛 ");
		gotoxy(126, 40);
		printf("  弛                                                          弛 ");
		gotoxy(126, 41);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");

		if (mouse_X >= 1146 && mouse_X <= 1390 && mouse_Y >= 740 && mouse_Y <= 840)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(140, 46);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(140, 47);
		printf("  弛                              弛 ");
		gotoxy(140, 48);
		printf("  弛                              弛 ");
		gotoxy(140, 49);
		printf("  弛           菴煎 陛晦          弛 ");
		gotoxy(140, 50);
		printf("  弛                              弛 ");
		gotoxy(140, 51);
		printf("  弛                              弛 ");
		gotoxy(140, 52);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
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
		if (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
			unitnum = 1;
			break;
		}
		else if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
			unitnum = 2;
			break;
		}
		else if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
			unitnum = 3;
			break;
		}
		else if (mouse_X >= 1146 && mouse_X <= 1390 && mouse_Y >= 740 && mouse_Y <= 840 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
			eraseScreen();
			return difSel();
		}
		if (GetAsyncKeyState(VK_LBUTTON) < 0)	k = 1;
	}
	eraseScreen();
	gotoxy(60, 12);
	printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
	gotoxy(60, 13);
	printf("  弛                                                          弛 ");
	gotoxy(60, 14);
	printf("  弛                                                          弛 ");
	gotoxy(60, 15);
	printf("  弛                                                          弛 ");
	gotoxy(60, 16);
	switch (unitnum) {
	case 1:
		printf("  弛               '嶸棉 1晦'陛 摹鷗腎歷蝗棲棻.               弛 ", difficulty);
		break;
	case 2:
		printf("  弛               '嶸棉 2晦'陛 摹鷗腎歷蝗棲棻.               弛 ", difficulty);
		break;
	case 3:
		printf("  弛               '嶸棉 3晦'陛 摹鷗腎歷蝗棲棻.               弛 ", difficulty);
		break;
	}
	gotoxy(60, 17);
	printf("  弛                                                          弛 ");
	gotoxy(60, 18);
	printf("  弛                     啪歜檜 衛濛腌棲棻.                   弛 ");
	gotoxy(60, 19);
	printf("  弛                                                          弛 ");
	gotoxy(60, 20);
	printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
	Sleep(1000);
	return;
}


void reset() {	//營衛濛
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

/* 罹晦憮睡攪 瞳 ぬ欐 л熱*/
void alltargetAttack() {	//餌寞縑憮 嶸棉戲煎 螃晦
	for (int i = 0; i < 1; i++) {
		createEnemy(Vulture, 1, Units[i].y, 8);
		createEnemy(Vulture, 640, Units[i].y, 24);
		createEnemy(Vulture, Units[i].x, 1, 16);
		createEnemy(Vulture, Units[i].x, 480, 0);
	}
}
void targetAttack() {	//楠渾縑憮 и寞щ戲煎 螃晦
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
void sbombAttack() {	//嶸棉 寞щ戲煎 3偃 翕衛奢問
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
void bbombAttack() {	//嶸棉 寞щ戲煎 5偃 翕衛奢問
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


void leftAttack(int x) {	//豭薹縑憮 螃朝 奢問 (爾蝶 1, 蝶纔檜雖 2 蛔)
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

void upAttack(int x) {	//嬪縑憮 螃朝 奢問 (爾蝶 1, 蝶纔檜雖 2 蛔)
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

void randomAttack() {	// 餌寞縑憮 楠渾戲煎 陳塭褥
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

void createHomingBomb() {	//嶸紫ァ驕 薯濛
	createEnemy(ScienceVessel, 1, 1, 0);
}

void homing() {	//嶸棉縑啪 嶸紫
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel) {
			if (Enemy[i].x < Units[0].x)	Enemy[i].x += difficulty + 1;
			else	Enemy[i].x -= (difficulty + 1);

			if (Enemy[i].y < Units[0].y)	Enemy[i].y += difficulty + 1;
			else	Enemy[i].y -= (difficulty + 1);

		}
	}
}

void homingAttack() {	//ァ驕縑憮 奢問 釭陛啪
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel && Enemy[i].isUsed == 1) {
			createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 0);
			createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 8);
			createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 16);
			createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 24);
		}
	}
}

void destroyHomingBomb() {	//ァ驕 餉-薯
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel && Enemy[i].isUsed == 1)	Enemy[i].isUsed = 0;
	}
}

void createBomb() {		//ァ驕 薯濛
	createEnemy(ScienceVessel, 30, 30, 0);
}

void moveBomb() {	//ァ驕 遺霜歜
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

void bombAttack() {	//ァ驕 奢問
	for (int i = 0; i < Unitcnt; i++) {
		if (Enemy[i].name == ScienceVessel && Enemy[i].isUsed == 1) {
			if (Enemy[i].temp == 0)				createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 16);
			else if (Enemy[i].temp == 1)	createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 24);
			else if (Enemy[i].temp == 2)	createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 0);
			else if (Enemy[i].temp == 3)	createEnemy(Vulture, Enemy[i].x, Enemy[i].y, 8);
		}
	}
}

void upFullAttack() {	//嬪縑憮 襞襞ж啪 螃朝 奢問
	int a = rand() % 30 - 29;
	for (int i = 0; i <= 1260; i += 75) {
		createEnemy(Vulture, i + a, 1, 16);
	}
}

void leftFullAttack() {	//豭薹縑憮 襞襞ж啪 螃朝 奢問
	int a = rand() % 30 - 29;
	for (int i = 0; i <= 480; i += 75) {
		createEnemy(Vulture, 1, i + a, 8);
	}
}
int Blast() {	//爾蝶 3 奢問 - 寰瞪雖渠 薯諼 ァだ 塽 錳 寰縑 氈朝雖 羹觼
	for (int i = 0; i < Unitcnt; i++) {
		if (Units[i].isUsed == 1) {
			for (int y = -Units[i].sizeY / 2 - 1; y <= Units[i].sizeY / 2; y++) {
				for (int x = -Units[i].sizeX / 2 - 1; x <= Units[i].sizeX / 2; x++) {
					if (Units[i].x + x <= 0 || Units[i].y + y <= 0 || Units[i].x + x >= 4096 || Units[i].y + y >= 4096)	continue;
					if (x >= -Units[i].sizeX / 2 && x < Units[i].sizeX / 2 && y >= -Units[i].sizeY && y < Units[i].sizeY / 2)	continue;
					if (fullMap[Units[i].x + x][Units[i].y + y] != 5) {
						for (int a = 0; a < 640; a++) {
							for (int b = 0; b < 480; b++) {
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

void startScreen() {	//衛濛�飛�
	gotoxy(70, 27);
	printf("VULTURE AVOID");
}

int gameClear() {

	gotoxy(60, 12);
	printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
	gotoxy(60, 13);
	printf("  弛                                                          弛 ");
	gotoxy(60, 14);
	printf("  弛                      蹴ж萄董棲棻!                       弛 ");
	gotoxy(60, 15);
	printf("  弛                                                          弛 ");
	gotoxy(60, 16);
	printf("  弛                                                          弛 ");
	gotoxy(60, 17);
	printf("  弛                 啪歜擊 贗葬橫ж樟蝗棲棻!                 弛 ");
	gotoxy(60, 18);
	printf("  弛                                                          弛 ");
	gotoxy(60, 19);
	printf("  弛                                                          弛 ");
	gotoxy(60, 20);
	switch (difficulty) {
	case 1:
		printf("  弛                      陪檜紫 : 蓮遺                       弛 ");
		break;
	case 2:
		printf("  弛                      陪檜紫 : 爾鱔                       弛 ");
		break;
	case 3:
		printf("  弛                     陪檜紫 : 橫溥遺                      弛 ");
		break;
	}
	gotoxy(60, 21);
	printf("  弛                                                          弛 ");
	gotoxy(60, 22);
	printf("  弛                                                          弛 ");
	gotoxy(60, 23);
	switch (unitnum) {
	case 1:
		printf("  弛                     嶸棉 偃熱 : 1晦                      弛 ");
		break;
	case 2:
		printf("  弛                     嶸棉 偃熱 : 2晦                      弛 ");
		break;
	case 3:
		printf("  弛                     嶸棉 偃熱 : 3晦                      弛 ");
		break;
	}
	gotoxy(60, 24);
	printf("  弛                                                          弛 ");
	gotoxy(60, 25);
	printf("  弛                                                          弛 ");
	gotoxy(60, 26);
	printf("  弛                                                          弛 ");
	gotoxy(60, 27);
	printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
	k = 1;
	while (1) {
		if (GetAsyncKeyState(VK_LBUTTON) >= 0)	k = 0;

		if (mouse_X >= 240 && mouse_X <= 710 && mouse_Y >= 570 && mouse_Y <= 690)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(27, 35);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(27, 36);
		printf("  弛                                                          弛 ");
		gotoxy(27, 37);
		printf("  弛                                                          弛 ");
		gotoxy(27, 38);
		printf("  弛                                                          弛 ");
		gotoxy(27, 39);
		printf("  弛                       1. 棻衛 ж晦                       弛 ");
		gotoxy(27, 40);
		printf("  弛                                                          弛 ");
		gotoxy(27, 41);
		printf("  弛                                                          弛 ");
		gotoxy(27, 42);
		printf("  弛                                                          弛 ");
		gotoxy(27, 43);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
		if (mouse_X >= 770 && mouse_X <= 1240 && mouse_Y >= 570 && mouse_Y <= 690)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(93, 35);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(93, 36);
		printf("  弛                                                          弛 ");
		gotoxy(93, 37);
		printf("  弛                                                          弛 ");
		gotoxy(93, 38);
		printf("  弛                                                          弛 ");
		gotoxy(93, 39);
		printf("  弛                       2. 啪歜 謙猿                       弛 ");
		gotoxy(93, 40);
		printf("  弛                                                          弛 ");
		gotoxy(93, 41);
		printf("  弛                                                          弛 ");
		gotoxy(93, 42);
		printf("  弛                                                          弛 ");
		gotoxy(93, 43);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
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
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                        1.啪歜 衛濛ж晦                   弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                       啪歜擊 衛濛м棲棻.                 弛 \n");
		printf("  弛                陪檜紫 撲薑 �飛橉虞� 剩橫骨棲棻.          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  弛                                                          弛 \n");
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");

		if (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

		gotoxy(63, 17);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(63, 18);
		printf("  弛                                                          弛 ");
		gotoxy(63, 19);
		printf("  弛                                                          弛 ");
		gotoxy(63, 20);
		printf("  弛                                                          弛 ");
		gotoxy(63, 21);
		printf("  弛                                                          弛 ");
		gotoxy(63, 22);
		printf("  弛                                                          弛 ");
		gotoxy(63, 23);
		printf("  弛                                                          弛 ");
		gotoxy(63, 24);
		printf("  弛                                                          弛 ");
		gotoxy(63, 25);
		printf("  弛                         2. 啪歜 撲貲憮                   弛 ");
		gotoxy(63, 26);
		printf("  弛                                                          弛 ");
		gotoxy(63, 27);
		printf("  弛                                                          弛 ");
		gotoxy(63, 28);
		printf("  弛                                                          弛 ");
		gotoxy(63, 29);
		printf("  弛                                                          弛 ");
		gotoxy(63, 30);
		printf("  弛                                                          弛 ");
		gotoxy(63, 31);
		printf("  弛                      啪歜曖 撲貲擊 瑣棲棻.               弛 ");
		gotoxy(63, 32);
		printf("  弛                                                          弛 ");
		gotoxy(63, 33);
		printf("  弛                                                          弛 ");
		gotoxy(63, 34);
		printf("  弛                                                          弛 ");
		gotoxy(63, 35);
		printf("  弛                                                          弛 ");
		gotoxy(63, 36);
		printf("  弛                                                          弛 ");
		gotoxy(63, 37);
		printf("  弛                                                          弛 ");
		gotoxy(63, 38);
		printf("  弛                                                          弛 ");
		gotoxy(63, 39);
		printf("  弛                                                          弛 ");
		gotoxy(63, 40);
		printf("  弛                                                          弛 ");
		gotoxy(63, 41);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");


		if (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(126, 17);
		printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
		gotoxy(126, 18);
		printf("  弛                                                          弛 ");
		gotoxy(126, 19);
		printf("  弛                                                          弛 ");
		gotoxy(126, 20);
		printf("  弛                                                          弛 ");
		gotoxy(126, 21);
		printf("  弛                                                          弛 ");
		gotoxy(126, 22);
		printf("  弛                                                          弛 ");
		gotoxy(126, 23);
		printf("  弛                                                          弛 ");
		gotoxy(126, 24);
		printf("  弛                                                          弛 ");
		gotoxy(126, 25);
		printf("  弛                         3. 啪歜 謙猿                     弛 ");
		gotoxy(126, 26);
		printf("  弛                                                          弛 ");
		gotoxy(126, 27);
		printf("  弛                                                          弛 ");
		gotoxy(126, 28);
		printf("  弛                                                          弛 ");
		gotoxy(126, 29);
		printf("  弛                                                          弛 ");
		gotoxy(126, 30);
		printf("  弛                                                          弛 ");
		gotoxy(126, 31);
		printf("  弛                       啪歜擊 謙猿м棲棻.                 弛 ");
		gotoxy(126, 32);
		printf("  弛                                                          弛 ");
		gotoxy(126, 33);
		printf("  弛                                                          弛 ");
		gotoxy(126, 34);
		printf("  弛                                                          弛 ");
		gotoxy(126, 35);
		printf("  弛                                                          弛 ");
		gotoxy(126, 36);
		printf("  弛                                                          弛 ");
		gotoxy(126, 37);
		printf("  弛                                                          弛 ");
		gotoxy(126, 38);
		printf("  弛                                                          弛 ");
		gotoxy(126, 39);
		printf("  弛                                                          弛 ");
		gotoxy(126, 40);
		printf("  弛                                                          弛 ");
		gotoxy(126, 41);
		printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");


		if (GetAsyncKeyState(0x31) < 0 || (mouse_X >= 22 && mouse_X <= 494 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0) && k == 0) {
			eraseScreen();
			gotoxy(60, 12);
			printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
			gotoxy(60, 13);
			printf("  弛                                                          弛 ");
			gotoxy(60, 14);
			printf("  弛                                                          弛 ");
			gotoxy(60, 15);
			printf("  弛                                                          弛 ");
			gotoxy(60, 16);
			printf("  弛               陪檜紫 撲薑 �飛橉虞� 剩橫骨棲棻.           弛 ");
			gotoxy(60, 17);
			printf("  弛                                                          弛 ");
			gotoxy(60, 18);
			printf("  弛                                                          弛 ");
			gotoxy(60, 19);
			printf("  弛                                                          弛 ");
			gotoxy(60, 20);
			printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
			Sleep(1000);
			eraseScreen();
			break;
		}
		else if (GetAsyncKeyState(0x32) < 0 || (mouse_X >= 530 && mouse_X <= 1000 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0) && k == 0) {
			eraseScreen();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			gotoxy(30, 6);
			printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
			gotoxy(30, 7);
			printf("  弛                                                                                                              弛 ");
			gotoxy(30, 8);
			printf("  弛                         檜 啪歜擎 1998喇縑 轎衛и Starcraft蒂 葬詭檜觼и 啪歜殮棲棻.                         弛 ");
			gotoxy(30, 9);
			printf("  弛                      檜 啪歜曖 輿檣奢擎 Starcraft曖 雖鼻 議葛攪 醞 ж釭檣 Medic殮棲棻.                       弛 ");
			gotoxy(30, 10);
			printf("  弛                         Medic擎 罹楝 寞щ縑憮 陳嬴螃朝 瞳檣 Vulture擊 Яп撿 м棲棻.                         弛 ");
			gotoxy(30, 11);
			printf("  弛                             陝 蝶纔檜雖葆棻 Vulture 朝 棻艇 ぬ欐戲煎 陳嬴褫棲棻.                             弛 ");
			gotoxy(30, 12);
			printf("  弛                     陳嬴螃朝 Vulture蒂 Яж雖 跤ж賊 Medic擎 避堅 啪歜擎 部釭啪 腌棲棻.                      弛 ");
			gotoxy(30, 13);
			printf("  弛                                                                                                              弛 ");
			gotoxy(30, 14);
			printf("  弛                        詭景縑憮朝 啪歜曖 陪檜紫諦 Medic曖 熱蒂 摹鷗й 熱 氈蝗棲棻.                           弛 ");
			gotoxy(30, 15);
			printf("  弛            陪檜紫朝 識 3欽啗煎 釭斯貊 陪檜紫縑 評塭 陳嬴螃朝 Vulture曖 熱諦 樓紫陛 棻落棲棻.                 弛 ");
			gotoxy(30, 16);
			printf("  弛    Medic擎 譆渠 3貲梱雖 Ы溯檜 й 熱 氈戲貊 葆辦蝶 萄楚斜 傳朝 酈爾萄曖 1,2,3 幗が戲煎 摹鷗й 熱 氈蝗棲棻.   弛 ");
			gotoxy(30, 17);
			printf("  弛             Medic擊 摹鷗ж堅 檜翕ж堅 談擎 嬪纂縑 葆辦蝶 辦贗葛擊 ж賊 詭蛐檜 斜夠戲煎 檜翕м棲棻.           弛 ");
			gotoxy(30, 18);
			printf("  弛            Medic罹楝 貲擊 翕衛縑 Ы溯檜 й ��朝 Medic郭葬 湊嬴紫 詭蛐擎 避堅, 啪歜擎 部檜 陶棲棻.            弛 ");
			gotoxy(30, 19);
			printf("  弛                                                                                                              弛 ");
			gotoxy(30, 20);
			printf("  弛                Vulture朝 и 蝶纔檜雖蒂 贗葬橫 й 陽葆棻 薯陝晦 棻艇 棟溘曖 爾蝶蒂 等溥褫棲棻.                弛 ");
			gotoxy(30, 21);
			printf("  弛                    Medic 檜 賅萇 Vulture 諦 爾蝶曖 奢問擊 Яж賊 啪歜擊 贗葬橫ж啪 腌棲棻.                   弛 ");
			gotoxy(30, 22);
			printf("  弛                                                                                                              弛 ");
			gotoxy(30, 23);
			printf("  弛                           薄熱葆棻 釭螃朝 蝶纔檜雖諦 爾蝶朝 棻擠婁 偽蝗棲棻.                                 弛 ");
			gotoxy(30, 24);
			printf("  弛                           - 蝶纔檜雖 1 : 0 ~ 2000                                                            弛 ");
			gotoxy(30, 25);
			printf("  弛                           - 爾蝶 1   : 2000 ~ 4000 (BattleCruiser)                                           弛 ");
			gotoxy(30, 26);
			printf("  弛                           - 蝶纔檜雖 2 : 4000 ~ 6000                                                         弛 ");
			gotoxy(30, 27);
			printf("  弛                           - 爾蝶 2   : 6000 ~ 8000 (ScienceVessel)                                           弛 ");
			gotoxy(30, 28);
			printf("  弛                           - 蝶纔檜雖 3 : 8000 ~ 10000                                                        弛 ");
			gotoxy(30, 29);
			printf("  弛                           - 爾蝶 3   : 10000 ~ 17000 (Ghost)                                                 弛 ");
			gotoxy(30, 30);
			printf("  弛                                                                                                              弛 ");
			gotoxy(30, 31);
			printf("  弛                                                                                                              弛 ");
			gotoxy(30, 32);
			printf("  弛                                                                                                              弛 ");
			gotoxy(30, 33);
			printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");

			while (1) {
				if (GetAsyncKeyState(VK_LBUTTON) >= 0)	k = 0;
				if (mouse_X >= 1146 && mouse_X <= 1390 && mouse_Y >= 740 && mouse_Y <= 840)	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				gotoxy(140, 46);
				printf("  忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
				gotoxy(140, 47);
				printf("  弛                              弛 ");
				gotoxy(140, 48);
				printf("  弛                              弛 ");
				gotoxy(140, 49);
				printf("  弛           菴煎 陛晦          弛 ");
				gotoxy(140, 50);
				printf("  弛                              弛 ");
				gotoxy(140, 51);
				printf("  弛                              弛 ");
				gotoxy(140, 52);
				printf("  戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
				if (mouse_X >= 1146 && mouse_X <= 1390 && mouse_Y >= 740 && mouse_Y <= 840 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0) {
					eraseScreen();
					return intro();
				}
				if (GetAsyncKeyState(VK_LBUTTON) < 0)	k = 1;
			}
			break;
		}
		else if (GetAsyncKeyState(0x33) < 0 || (mouse_X >= 1030 && mouse_X <= 1504 && mouse_Y >= 280 && mouse_Y <= 660 && GetAsyncKeyState(VK_LBUTTON) < 0 && k == 0)) {
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
	sounds[4].isused = 1;	//嶸棉 儅撩衛 模葬
	for (int i = 0; i < unitnum; i++) {
		createUnit(Medic, i * 100 + 150, i * 100 + 150);	//嶸棉 偎熱虜躑 儅撩
	}
	score = 0;

	while (1)
	{

		gotoxy(0, 0);
		long long oldTime = GetTickCount64();
		score++;
		// 蝶纔檜雖 塽 爾蝶 ぬ欐 撲薑	樂霜�� 罹晦 輿戮擎 覬...冗冗
		if (score >= 0 && score < 2000) {	//stage 1

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
			if (score % 500 == 0)	blast = 1, blastx = rand() % 300 + 200, blasty = rand() % 240 + 120, sounds[0].isused = 1;
			if (score % 500 == 500 - 100 * difficulty) {
				blast = 0;
				sounds[5].isused = 1;
				if (Blast() == 1)	gameover = 1;
			}

			if (score < 12500) {
				if (score % 40 - 5 * difficulty == 0)	randomAttack();
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
				if (score % 230 - 7 * difficulty == 0)	leftAttack(1), upAttack(1);
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
			if (score % 73 - 5 * difficulty == 0)	bombAttack();
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
		//嶸棉 醱給 羹觼 塽 啪歜 謙猿/營衛濛
		for (int i = 0; i < unitnum; i++) {
			if (collisionCheck(i) == 1 || gameover == 1) {
				sounds[3].isused = 1;
				for (int j = 222; j < 230; j++) {
					for (int i = 0; i < unitnum; i++) {
						Units[i].Death = j;
					}
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

		LinearMove();	// 嶸棉 檜翕


		initxy();	//寰虜菟橫霞 嶸棉 xy 衡啖幗葬晦
		initCursor();	//醴憮 梭緇檜晦 橈擁晦
		inMapCheck();	//裘 婦溼 л熱
		processInput();		//檣Е Щ煎撮諒

		//滓羹觼
		paint(hWnd);
		long long nowTime = GetTickCount64();
		if (nowTime - oldTime <= 16)
			Sleep(16 - (nowTime - oldTime));
	}
}