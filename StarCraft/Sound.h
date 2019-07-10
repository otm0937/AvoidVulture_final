/* 소리 관련*/
#pragma once

#define BUFFER_COUNT 8
#define BUFFER_SIZE 1024

typedef struct Sound {
	unsigned int isused;
	uint8_t*soundNest;
	FILE *fstream;
	uint8_t *wavdata;
	int wavLength;
	int wavPos;

}Sound;

Sound backgroundMusic;
Sound sounds[100];