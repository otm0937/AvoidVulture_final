#include "header.h"
#include "terran.h"
#include "UI.h"
#include "UnitAnimation.h"
#include "tile.h"

int bossx = 640;	//
void drawBoss(wchar_t *text, int len, int y, int x, int h, int w, UINT flags, uint_fast32_t color, unsigned int screen[480][640]) {	//보스 텍스트 보여주기
	static HFONT font = NULL;
	if (font == NULL)
		font = CreateFont(90, 25, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FF_MODERN | FIXED_PITCH,
			TEXT("맑은 고딕"));

	uint32_t(*ptr)[640];
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = 640;
	bmi.bmiHeader.biHeight = -480;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	HDC hdc = CreateCompatibleDC(NULL);
	HBITMAP bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&ptr, NULL, 0);
	HGDIOBJ oldObject = SelectObject(hdc, bitmap);

	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));
	HGDIOBJ oldFont = SelectObject(hdc, font);

	RECT r;
	r.left = 0;
	r.top = 0;
	r.bottom = h;
	r.right = w;
	DrawText(hdc, text, len, &r, flags);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			if (ptr[i][j])
				screen[i + y][j + x] = color;

	SelectObject(hdc, oldFont);
	SelectObject(hdc, oldObject);
	DeleteObject(bitmap);
	DeleteDC(hdc);
}

void drawScore(wchar_t *text, int len, int y, int x, int h, int w, UINT flags, uint_fast32_t color, unsigned int screen[480][640]) {	//점수 출력
	static HFONT font = NULL;
	if (font == NULL)
		font = CreateFont(30, 11, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, FF_MODERN | FIXED_PITCH,
			TEXT("맑은 고딕"));

	uint32_t(*ptr)[640];
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = 640;
	bmi.bmiHeader.biHeight = -480;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	HDC hdc = CreateCompatibleDC(NULL);
	HBITMAP bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&ptr, NULL, 0);
	HGDIOBJ oldObject = SelectObject(hdc, bitmap);

	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));
	HGDIOBJ oldFont = SelectObject(hdc, font);

	RECT r;
	r.left = 0;
	r.top = 0;
	r.bottom = h;
	r.right = w;
	DrawText(hdc, text, len, &r, flags);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			if (ptr[i][j])
				screen[i + y][j + x] = color;

	SelectObject(hdc, oldFont);
	SelectObject(hdc, oldObject);
	DeleteObject(bitmap);
	DeleteDC(hdc);
}


void inMapCheck()	//맵 관련 함수
{
	int mousecheck = 0;
	for (int i = 0; i < 640; i++) {	//맵 비우기
		for (int j = 0; j < 480; j++) {
			if (fullMap[i][j]!=5)	fullMap[i][j] = 0;
		}
	}
	for (int i = 0; i < Unitcnt; i++)	//유닛 맵 내부 확인
	{
		if (!Units[i].isUsed&&!Enemy[i].isUsed)
			continue;
		if (Units[i].x > scroll_X - 200 && Units[i].x < scroll_X + graphicWidth + 200
			&& Units[i].y > scroll_Y - 200 && Units[i].y < scroll_Y + graphicHeight + 200)
			Units[i].inMap = 1;
		else
			Units[i].inMap = 0;

		if (Enemy[i].x > scroll_X - 200 && Enemy[i].x < scroll_X + graphicWidth + 200
			&& Enemy[i].y > scroll_Y - 200 && Enemy[i].y < scroll_Y + graphicHeight + 200)
			Enemy[i].inMap = 1;
		else
			Enemy[i].inMap = 0;


		if (Enemy[i].isUsed) {	//유닛 맵에 생성 (그리는거 아니고 그냥 생성)
			if (Enemy[i].direction == 0) {
				for (int y = -Enemy[i].sizeY / 2+2; y < Enemy[i].sizeY / 2-6; y++) {
					for (int x = -Enemy[i].sizeX / 2; x < Enemy[i].sizeX / 2; x++) {
						if (Enemy[i].x + x <= 0 || Enemy[i].y + y <= 0 || Enemy[i].x + x >= 4096 || Enemy[i].y + y >= 4096)	continue;
						fullMap[Enemy[i].x + x][Enemy[i].y + y] = 4;
					}
				}
			}
			else if (Enemy[i].direction == 8) {
				for (int y = -Enemy[i].sizeY / 2; y < Enemy[i].sizeY / 2; y++) {
					for (int x = -Enemy[i].sizeX / 2-2; x < Enemy[i].sizeX / 2+6; x++) {
						if (Enemy[i].x + x <= 0 || Enemy[i].y + y <= 0 || Enemy[i].x + x >= 4096 || Enemy[i].y + y >= 4096)	continue;
						fullMap[Enemy[i].x + x][Enemy[i].y + y] = 4;
					}
				}
			}
			else if (Enemy[i].direction == 16) {
				for (int y = -Enemy[i].sizeY / 2+4; y < Enemy[i].sizeY / 2+2; y++) {
					for (int x = -Enemy[i].sizeX / 2; x < Enemy[i].sizeX / 2; x++) {
						if (Enemy[i].x + x <= 0 || Enemy[i].y + y <= 0 || Enemy[i].x + x >= 4096 || Enemy[i].y + y >= 4096)	continue;
						fullMap[Enemy[i].x + x][Enemy[i].y + y] = 4;
					}
				}
			}
			else {
				for (int y = -Enemy[i].sizeY / 2; y < Enemy[i].sizeY / 2; y++) {
					for (int x = -Enemy[i].sizeX / 2-4; x < Enemy[i].sizeX / 2-2; x++) {
						if (Enemy[i].x + x <= 0 || Enemy[i].y + y <= 0 || Enemy[i].x + x >= 4096 || Enemy[i].y + y >= 4096)	continue;
						fullMap[Enemy[i].x + x][Enemy[i].y + y] = 4;
					}
				}
			}
		}

		if (Units[i].isUsed) {	//유닛도 생성
			for (int y = -Units[i].sizeY / 2; y < Units[i].sizeY / 2; y++) {
				for (int x = -Units[i].sizeX / 2; x < Units[i].sizeX / 2; x++) {
					if (Units[i].x + x <= 0 || Units[i].y + y <= 0 || Units[i].x + x >= 4096 || Units[i].y + y >= 4096)	continue;
					fullMap[Units[i].x + x][Units[i].y + y] = 1;
				}
			}
		}

		if (Units[i].x - Units[i].sizeX / 2 <= GDImouse_X && Units[i].x + Units[i].sizeX / 2 >= GDImouse_X &&
			Units[i].y - Units[i].sizeY / 2 <= GDImouse_Y && Units[i].y + Units[i].sizeY / 2 >= GDImouse_Y)
			mousecheck = 1;
	}
	if (mousecheck == 1)	//마우스 종류
		mouseMode = magg;
	else
		mouseMode = arrow;
}

//유닛 그리기
void paintUnits(unsigned int screen[graphicHeight][graphicWidth], unsigned int Unitimg[][bmpSize][bmpSize], int X, int Y, int direction, int animation) {
	for (int i = 0; i < bmpSize; i++) {
		if (0 <= i + Y - scroll_Y - (bmpSize / 2) && i + Y - scroll_Y - (bmpSize / 2) < graphicHeight)
			for (int j = 0; j < bmpSize; j++)
				if (direction > 16) {
					if (0 <= (bmpSize - 1) - j + X - scroll_X - (bmpSize / 2) && (bmpSize - 1) - j + X - scroll_X - (bmpSize / 2) < graphicWidth)
						if ((Unitimg[(animation * 17) + 32 - direction][i][j] & 0xFFFFFF) != 0x0000ff)
							screen[i + Y - scroll_Y - (bmpSize / 2)][(bmpSize - 1) - j + X - scroll_X - (bmpSize / 2)] = Unitimg[(animation * 17) + 32 - direction][i][j];
				}
				else {
					if (0 <= j + X - scroll_X - (bmpSize / 2) && j + X - scroll_X - (bmpSize / 2) < graphicWidth)
						if ((Unitimg[(animation * 17) + direction][i][j] & 0xFFFFFF) != 0x0000ff)
							screen[i + Y - scroll_Y - (bmpSize / 2)][j + X - scroll_X - (bmpSize / 2)] = Unitimg[(animation * 17) + direction][i][j];
				}
	}
}

//지상유닛 그리기
void paintGroundUnits(unsigned int screen[graphicHeight][graphicWidth]) {	//유닛 그리기 (원래는 다른 유닛 모두 있는데, 이 게임에는 메딕만 등장해서..
	for (int n = 0; n < Unitcnt; n++) {
		if (Units[n].isUsed&&Units[n].inMap) {
			switch (Units[n].name) {
			case Medic:

				if (Units[n].Death > 0) {
					Units[n].animation = Units[n].Death;
					paintUnits(screen, Medicimg, Units[n].x, Units[n].y, 1, Units[n].animation);
					continue;
				}

				if (Units[n].moving) {
					if (Units[n].animation < MedicWork * 4 || Units[n].animation >= MedicWork_ * 4)
						Units[n].animation = MedicWork * 4;
				}
				else
					Units[n].animation = MedicStay;
				paintUnits(screen, Medicimg, Units[n].x, Units[n].y, Units[n].direction, Units[n].animation / 4);
				Units[n].animation++;
			}
		}
	}
}

//지상유닛 (적) 그리기
void paintGroundEnemy(unsigned int screen[graphicHeight][graphicWidth]) {
	for (int n = 0; n < Unitcnt; n++) {
		if (Enemy[n].isUsed&&Enemy[n].inMap) {
			switch (Enemy[n].name) {
			break; case Ghost:
				paintUnits(screen, Ghostimg, Enemy[n].x, Enemy[n].y, Enemy[n].direction,0);
			break; case Vulture:
				paintUnits(screen, Vultureimg, Enemy[n].x, Enemy[n].y, Enemy[n].direction,0);
			break; case BattleCruiser:
				paintUnits(screen, BattleCruiserimg, Enemy[n].x, Enemy[n].y, Enemy[n].direction,0);
			break; case ScienceVessel:
				paintUnits(screen, ScienceVesselimg, Enemy[n].x, Enemy[n].y, Enemy[n].direction,0);
				//paintUnits(screen, ScienceVesselTimg, Enemy[n].x, Enemy[n].y, Enemy[n].directionTurret,1);
				break;
			}
		}
	}
}


void cursors(unsigned int screen[graphicHeight][graphicWidth], unsigned int mouseimg[][bmpSize][bmpSize]) {	//커서 그리기
	for (int i = 0; i < bmpSize; i++)
		if (0 <= i + GDImouse_Y - (bmpSize / 2) && i + GDImouse_Y - (bmpSize / 2) < graphicHeight)
			for (int j = 0; j < bmpSize; j++)
				if (0 <= j + GDImouse_X - (bmpSize / 2) && j + GDImouse_X - (bmpSize / 2) < graphicWidth)
					if ((mouseimg[mouseLoop][i][j] & 0xFFFFFF) != 0x0000ff)
						screen[i + GDImouse_Y - (bmpSize / 2)][j + GDImouse_X - (bmpSize / 2)] = (mouseimg[mouseLoop][i][j]);
	mouseLoop++;
}

void paintMouseCursor(unsigned int screen[graphicHeight][graphicWidth]) {	//마우스 커서 종류
	
	switch (mouseMode) {
	case arrow:				if (mouseLoop >= arrowImgCount)		mouseLoop = 0; cursors(screen, arrowimg);
	break; case drag:		if (mouseLoop >= dragImgCount)		mouseLoop = 0; cursors(screen, dragimg);
	break; case magg:		if (mouseLoop >= maggImgCount)		mouseLoop = 0; cursors(screen, maggimg);
	break; case magr:		if (mouseLoop >= magrImgCount)		mouseLoop = 0; cursors(screen, magrimg);
	break; case magy:		if (mouseLoop >= magyImgCount)		mouseLoop = 0; cursors(screen, magyimg);

	break; case scrolld:	if (mouseLoop >= scrolldImgCount)	mouseLoop = 0; cursors(screen, scrolldimg);
	break; case scrolldl:	if (mouseLoop >= scrolldlImgCount)	mouseLoop = 0; cursors(screen, scrolldlimg);
	break; case scrolldr:	if (mouseLoop >= scrolldrImgCount)	mouseLoop = 0; cursors(screen, scrolldrimg);
	break; case scrolll:	if (mouseLoop >= scrolllImgCount)	mouseLoop = 0; cursors(screen, scrolllimg);
	break; case scrollr:	if (mouseLoop >= scrollrImgCount)	mouseLoop = 0; cursors(screen, scrollrimg);
	break; case scrollu:	if (mouseLoop >= scrolluImgCount)	mouseLoop = 0; cursors(screen, scrolluimg);
	break; case scrollul:	if (mouseLoop >= scrollulImgCount)	mouseLoop = 0; cursors(screen, scrollulimg);
	break; case scrollur:	if (mouseLoop >= scrollurImgCount)	mouseLoop = 0; cursors(screen, scrollurimg);

	break; case targg:		if (mouseLoop >= targgImgCount)		mouseLoop = 0; cursors(screen, targgimg);
	break; case targr:		if (mouseLoop >= targrImgCount)		mouseLoop = 0; cursors(screen, targrimg);
	break; case targy:		if (mouseLoop >= targyImgCount)		mouseLoop = 0; cursors(screen, targyimg);
	break; case targn:		if (mouseLoop >= targnImgCount)		mouseLoop = 0; cursors(screen, targnimg);
	break;
	}
}


void paintDrag(unsigned int screen[graphicHeight][graphicWidth], int dis_X, int dis_Y, int mid_X, int mid_Y) {	//드래그 그리기
	gotoxy(0, 30);
	for (int i = 0; i < graphicWidth; i++) {
		for (int j = 0; j < graphicHeight; j++) {
			if ((mid_X - dis_X <= i + scroll_X && mid_X + dis_X >= i + scroll_X && mid_Y + dis_Y == j + scroll_Y) || (mid_X - dis_X <= i + scroll_X && mid_X + dis_X >= i + scroll_X && mid_Y - dis_Y == j + scroll_Y) ||
				(mid_Y - dis_Y <= j + scroll_Y && mid_Y + dis_Y >= j + scroll_Y && mid_X + dis_X == i + scroll_X) || (mid_Y - dis_Y <= j + scroll_Y && mid_Y + dis_Y >= j + scroll_Y && mid_X - dis_X == i + scroll_X))
				screen[j][i] = 0x00ff00;
		}

	}
}

void paintBackground1(unsigned int screen[graphicHeight][graphicWidth]) {	//스테이지 1 배경
	for (int a = 0; a <= 768; a += 128)
		for (int b = 0; b <= 708; b += 128)
			for (int i = 0; i < bmpSize; i++)
				if (0 <= i + b - scroll_Y - (bmpSize / 2) && i + b - scroll_Y - (bmpSize / 2) < graphicHeight)
					for (int j = 0; j < bmpSize; j++)
						if (0 <= j + a - scroll_X - (bmpSize / 2) && j + a - scroll_X - (bmpSize / 2) < graphicWidth)
							screen[i + b - scroll_Y - (bmpSize / 2)][j + a - scroll_X - (bmpSize / 2)] = mapBlackimg[i][j];
}

void paintBackground2(unsigned int screen[graphicHeight][graphicWidth]) {	//스테이지 2 배경
	for (int a = 0; a <= 768; a += 128)
		for (int b = 0; b <= 708; b += 128)
			for (int i = 0; i < bmpSize; i++)
				if (0 <= i + b - scroll_Y - (bmpSize / 2) && i + b - scroll_Y - (bmpSize / 2) < graphicHeight)
					for (int j = 0; j < bmpSize; j++)
						if (0 <= j + a - scroll_X - (bmpSize / 2) && j + a - scroll_X - (bmpSize / 2) < graphicWidth)
							screen[i + b - scroll_Y - (bmpSize / 2)][j + a - scroll_X - (bmpSize / 2)] = mapWhiteimg[i][j];
}

void paintBackground3(unsigned int screen[graphicHeight][graphicWidth]) {	//스테이지 3 배경
	for (int a = 0; a <= 768; a += 128)
		for (int b = 0; b <= 708; b += 128)
			for (int i = 0; i < bmpSize; i++)
				if (0 <= i + b - scroll_Y - (bmpSize / 2) && i + b - scroll_Y - (bmpSize / 2) < graphicHeight)
					for (int j = 0; j < bmpSize; j++)
						if (0 <= j + a - scroll_X - (bmpSize / 2) && j + a - scroll_X - (bmpSize / 2) < graphicWidth)
							screen[i + b - scroll_Y - (bmpSize / 2)][j + a - scroll_X - (bmpSize / 2)] = mapBlackWhiteimg[i][j];
}
void DrawEllipse(unsigned int screen[graphicHeight][graphicWidth])	//타원 그리기 (캐릭터 선택시)
{
	int x, y, sigma;
	for (int i = 0; i < Unitcnt; i++) {
		if (Units[i].selected == 1) {
			int xc = Units[i].x, yc = Units[i].y, width = Units[i].sizeX / 3 * 2, height = width / 4 * 3;
			int a2 = width * width;
			int b2 = height * height;
			int fa2 = 4 * a2, fb2 = 4 * b2;
			/* first half */
			for (x = 0, y = height, sigma = 2 * b2 + a2 * (1 - 2 * height); b2*x <= a2 * y; x++)
			{
				if (yc - y + 10 - scroll_Y > 0 && yc + y + 10 - scroll_Y < graphicHeight && xc - x - scroll_X > 0 && xc + x - scroll_X < graphicWidth)
				{
					screen[yc + y + 10 - scroll_Y][xc + x - scroll_X] = 0x00ff00;
					screen[yc + y + 10 - scroll_Y][xc - x - scroll_X] = 0x00ff00;
					screen[yc - y + 10 - scroll_Y][xc + x - scroll_X] = 0x00ff00;
					screen[yc - y + 10 - scroll_Y][xc - x - scroll_X] = 0x00ff00;
				}
				if (sigma >= 0)
				{
					sigma += fa2 * (1 - y);
					y--;
				}
				sigma += b2 * ((4 * x) + 6);
			}

			/* second half */
			for (x = width, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * width); a2*y <= b2 * x; y++)
			{
				if (yc - y + 10 - scroll_Y >= 0 && yc + y + 10 - scroll_Y < graphicHeight && xc - x - scroll_X >= 0 && xc + x - scroll_X < graphicWidth)
				{
					screen[yc + y + 10 - scroll_Y][xc + x - scroll_X] = 0x00ff00;
					screen[yc + y + 10 - scroll_Y][xc - x - scroll_X] = 0x00ff00;
					screen[yc - y + 10 - scroll_Y][xc + x - scroll_X] = 0x00ff00;
					screen[yc - y + 10 - scroll_Y][xc - x - scroll_X] = 0x00ff00;
				}
				if (sigma >= 0)
				{
					sigma += fb2 * (1 - x);
					x--;
				}
				sigma += a2 * ((4 * y) + 6);
			}
		}
	}

}



void drawCircle(unsigned int screen[graphicHeight][graphicWidth], int r, int cx, int cy, int type)	//원 그리기(보스3 Blast 패턴)
{
	int x, y;
	int p;
	int k = 0x00ff00;
	if (type == 2)	k = 0xff0000;
	x = 0;
	y = r;
	p = 1 - r;

	fullMap[x + cx][y + cy] = 5;
	fullMap[x + cx][-y + cy] = 5;
	fullMap[y + cx][x + cy] = 5;
	fullMap[-y + cx][x + cy] = 5;
	screen[y + cy][x + cx] = k;
	screen[-y + cy][x + cx] = k;
	screen[x + cy][y + cx] = k;
	screen[x + cy][-y + cx] = k;
	x = 1;
	while (x < y) {

		if (p < 0) {
			p += x + x + 1;
		}
		else {
			p += x + x + 1 - y - y;
			--y;
			for (int i = 0; i < x; i++) {
				screen[y + cy][i + cx] = k;
				screen[-y + cy][i + cx] = k;
				screen[y + cy][-i + cx] = k;
				screen[-y + cy][-i + cx] = k;
				screen[i + cy][y + cx] = k;
				screen[-i + cy][y + cx] = k;
				screen[i + cy][-y + cx] = k;
				screen[-i + cy][-y + cx] = k;

				fullMap[i + cx][y + cy] = 5;
				fullMap[i + cx][-y + cy] = 5;
				fullMap[-i + cx][y + cy] = 5;
				fullMap[-i + cx][-y + cy] = 5;
				fullMap[y + cx][i + cy] = 5;
				fullMap[y + cx][-i + cy] = 5;
				fullMap[-y + cx][i + cy] = 5;
				fullMap[-y + cx][-i + cy] = 5;
			}
		}
		screen[y + cy][x + cx] = k;
		screen[-y + cy][x + cx] = k;
		screen[y + cy][-x + cx] = k;
		screen[-y + cy][-x + cx] = k;
		screen[x + cy][y + cx] = k;
		screen[-x + cy][y + cx] = k;
		screen[x + cy][-y + cx] = k;
		screen[-x + cy][-y + cx] = k;


		fullMap[x + cx][y + cy] = 5;
		fullMap[x + cx][-y + cy] = 5;
		fullMap[-x + cx][y + cy] = 5;
		fullMap[-x + cx][-y + cy] = 5;
		fullMap[y + cx][x + cy] = 5;
		fullMap[y + cx][-x + cy] = 5;
		fullMap[-y + cx][x + cy] = 5;
		fullMap[-y + cx][-x + cy] = 5;

		++x;
	}

	for (int i = cy - y + 1; i < cy + y; i++) {
		for (int j = cx - y + 1; j < cx + y; j++) {
			screen[i][j] = k;
			fullMap[j][i] = 5;
		}
	}
}
void drawRectangle(unsigned int *screen[graphicHeight][graphicWidth], int x1, int y1, int x2, int y2) {
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			printf("%d %d\n", i, j);
			if (i == x1 || i == x2 || j == y1 || j == y2)	screen[j][i] = 0xff0000;
		}
	}
}

void paint(HWND hWnd) {
	RECT screen;
	GetClientRect(hWnd, &screen);
	pixelWidth = screen.right - screen.left;
	pixelHeight = screen.bottom - screen.top;
	screen.top = screen.top * dpi / 96;
	screen.bottom = screen.bottom * dpi / 96;
	screen.left = screen.left * dpi / 96;
	screen.right = screen.right * dpi / 96;

	int screenWidth;
	int screenHeight;
	int consoleWidth = screen.right - screen.left;
	int consoleHeight = screen.bottom - screen.top;
	int a = consoleHeight * 4 / 3;
	int b = consoleWidth * 3 / 4;





	if (a < screen.right - screen.left)
	{
		screenWidth = a;
		screenHeight = screen.bottom - screen.top;
	}
	else
	{
		screenWidth = screen.right - screen.left;
		screenHeight = b;
	}
	HorizonBorder = (consoleWidth - screenWidth) * 96 / dpi / 2;
	VerticalBorder = (consoleHeight - screenHeight) * 96 / dpi / 2;

	HDC hdc = GetDC(hWnd);

	BITMAPINFOHEADER bitmapinfo;
	bitmapinfo.biSize = sizeof(BITMAPINFO);
	bitmapinfo.biHeight = -graphicHeight;
	bitmapinfo.biWidth = graphicWidth;
	bitmapinfo.biPlanes = 1;
	bitmapinfo.biBitCount = 32;
	bitmapinfo.biCompression = BI_RGB;

	HBITMAP hbitmap = CreateDIBSection(hdc, &bitmapinfo, DIB_RGB_COLORS, &img, NULL, 0);
	HDC memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, hbitmap);

	if (score <=4000)	paintBackground1(img);
	else if (score <= 8000)	paintBackground2(img);
	else	paintBackground3(img);
	// 보스 3 패턴
	if (blast == 1)	drawCircle(img, 100, blastx, blasty, blast);
	if (blast == 2)	drawCircle(img, 150, blastx, blasty, blast);
	DrawEllipse(img);
	paintGroundUnits(img);
	paintGroundEnemy(img);

	wchar_t text[100];
	if (score >= 2200 && score <= 2300) {
		swprintf(text, 100, L"BOSS : BATTLECRUISER");
		drawBoss(text, -1, 0, 10, 300, 792, 0, 0xff0000, img);
		bossx -= 3;
	}
	else if (score >= 6200 && score <= 6300) {
		swprintf(text, 100, L"BOSS : SCIENCE VESSEL");
		drawBoss(text, -1, 0, 10, 300, 792, 0, 0xff0000, img);
		bossx -= 3;
	}
	else if (score >= 10100 && score <= 10200) {
		swprintf(text, 100, L"BOSS : GHOST");
		drawBoss(text, -1, 0, 10, 300, 792, 0, 0xff0000, img);
		bossx -= 3;
	}
	else {
		if (score < 0)	swprintf(text, 100, L"점수 : %d", 0);
		else	swprintf(text, 100, L"점수 : %d", score);
		drawScore(text, -1, 0, 250, 200, 500, 0, 0xffffff, img);
	}

	if (click > 1)
	{
		paintDrag(img, abs(start_x - ((start_x + GDImouse_X + scroll_X) / 2)),
			abs(start_y - ((start_y + GDImouse_Y + scroll_Y) / 2)),
			(start_x + GDImouse_X + scroll_X) / 2,
			(start_y + GDImouse_Y + scroll_Y) / 2);
	}
	paintMouseCursor(img);




	//SetStretchBltMode(hdc, HALFTONE); 블러 처리
	//SetBrushOrgEx(hdc, 0, 0, NULL);
	StretchBlt(hdc, (consoleWidth - screenWidth) / 2, (consoleHeight - screenHeight) / 2, screenWidth, screenHeight, memdc, 0, 0, graphicWidth, graphicHeight, SRCCOPY);

	DeleteDC(memdc);
	DeleteObject(hbitmap);
	ReleaseDC(hWnd, hdc);
}