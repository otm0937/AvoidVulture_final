#define _CRT_SECURE_NO_WARNNINGS
#pragma warning(disable: 4996)
#pragma comment (lib, "winmm.lib") // sound
#include <stdio.h>
#include <Windows.h>
#include <windowsx.h>
#include <stdint.h>
#include <wchar.h>
#include <io.h>
#include <time.h>
#include <mmsystem.h>  // mciSendString() 
#include <conio.h>
#pragma once


#define mapPixelSize 4096
#define graphicWidth 640
#define graphicHeight 480
#define bmpSize 128
#define mapSize 128

INPUT_RECORD rec;
DWORD dwNOER;

int selected;
int gameover;
int score;
int difficulty;
int unitnum;
int blastX;
int blastY;
int blastr;	
int dpi;
int scroll_X;
int scroll_Y;
int mouse_X;
int mouse_Y;
int GDImouse_X;
int GDImouse_Y;
int pixelWidth;
int pixelHeight;
int global_width;
int global_height;
int HorizonBorder;
int VerticalBorder;
int blast;
int blastx, blasty;
unsigned int map[mapSize][mapSize];
unsigned int regionalMap[128][128];
unsigned int inregionMap[128][128];
unsigned int fullMap[4096][4096];
unsigned int mapChecker[4096][4096];
int mouseMode;
int mouseLoop;
int click;
int start_x, start_y;
int start;
int o, c, k;
void *img;