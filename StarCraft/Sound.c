/* À½¾Ç °ü·Ã ÇÔ¼ö */
#include "header.h"
#include "Sound.h"







struct WaveHeader {
	char riff[4]; //literal "RIFF"
	uint32_t riff_filesize;
	char riff_type[4]; //literal "WAVE"

	char fmt[4]; //literal "fmt "
	uint32_t formatLength;
	uint16_t format_format; //type. 1=PCM, 3=float
	uint16_t format_channels;
	uint32_t format_sampleRate;
	uint32_t format_byteRate;
	uint16_t format_blockAlign;
	uint16_t format_bitsPerSample;
};


int validateWavFile(struct WaveHeader *w) {
	if (w->riff[0] != 'R' || w->riff[1] != 'I' || w->riff[2] != 'F' || w->riff[3] != 'F')
		return 0;
	if (w->riff_type[0] != 'W' || w->riff_type[1] != 'A' || w->riff_type[2] != 'V' || w->riff_type[3] != 'E')
		return 0;
	if (w->fmt[0] != 'f' || w->fmt[1] != 'm' || w->fmt[2] != 't' || w->fmt[3] != ' ')
		return 0;

	if (w->format_channels != 1)
		return 0;
	if (w->format_format != 1)
		return 0;
	if (w->format_bitsPerSample != 8)
		return 0;

	return 1;
}

static uint8_t* getWavData(uint8_t *stream, uint32_t *psize) {
	const static char sig[] = { 'd', 'a', 't', 'a' };

	int i = 4;
	while (stream[i] != sig[0] || stream[i + 1] != sig[1] || stream[i + 2] != sig[2] || stream[i + 3] != sig[3])
		i++;
	i += 4;

	uint32_t *const size = (uint32_t*)(stream + i);
	if (psize != NULL)
		*psize = *size;
	i += 4;

	return stream + i;
}

uint8_t* loadWavFile(int8_t *data, uint32_t *size) {
	struct WaveHeader w;
	memcpy(&w, data, sizeof(struct WaveHeader));
	if (!validateWavFile(&w))
		return NULL;

	uint32_t filesize;
	uint8_t *pcm = getWavData(data, &filesize);

	*size = filesize;
	return pcm;
}

void bgmLoad(char*filename) {
	int filesize;
	backgroundMusic.fstream = fopen(filename, "rb");
	fseek(backgroundMusic.fstream, 0, SEEK_END);
	filesize = ftell(backgroundMusic.fstream);
	fseek(backgroundMusic.fstream, 0, SEEK_SET);
	backgroundMusic.soundNest = malloc(filesize);
	fread(backgroundMusic.soundNest, 1, filesize, backgroundMusic.fstream);

	backgroundMusic.wavdata = loadWavFile(backgroundMusic.soundNest, &backgroundMusic.wavLength);
}

void sndLoad(char*filename, int i) {
	int filesize;
	sounds[i].fstream = fopen(filename, "rb");
	fseek(sounds[i].fstream, 0, SEEK_END);
	filesize = ftell(sounds[i].fstream);
	fseek(sounds[i].fstream, 0, SEEK_SET);
	sounds[i].soundNest = malloc(filesize);
	fread(sounds[i].soundNest, 1, filesize, sounds[i].fstream);

	sounds[i].wavdata = loadWavFile(sounds[i].soundNest, &sounds[i].wavLength);
}

DWORD __stdcall Soundmain(void*p)
{
	
	bgmLoad("Sounds/protoss1.wav");//¹è°æ
	sndLoad("Sounds/tadupd04.wav", 0);//´ºÅ¬
	sndLoad("Sounds/tbardy00.wav", 1);//¹èÆ²
	sndLoad("Sounds/tghrdy00.wav", 2);//°íšÀ
	sndLoad("Sounds/TMdDth00.wav", 3);//¸ÞµñÁÞÀº
	sndLoad("Sounds/TMdRdy00.wav", 4);//¸Þµñ »ý¼º
	sndLoad("Sounds/tnshit00.wav", 5);//Æø!¹ß!
	sndLoad("Sounds/tverdy00.wav", 6);//»çºªÀ»
	


	int flagRunSound = 1;

	PCMWAVEFORMAT format;
	format.wf.wFormatTag = WAVE_FORMAT_PCM;
	format.wf.nChannels = 1;
	format.wf.nSamplesPerSec = 48000;
	format.wf.nAvgBytesPerSec = 2 * 48000;
	format.wBitsPerSample = 2 * 8;
	format.wf.nBlockAlign = 2;

	HWAVEOUT hwo;
	MMRESULT result = waveOutOpen(&hwo, WAVE_MAPPER, &format, (DWORD_PTR)NULL, (DWORD_PTR)NULL, 0);
	if (result != 0)
		return 1;

	int bufIdx = 0;
	WAVEHDR buf[BUFFER_COUNT];
	for (int i = 0; i < BUFFER_COUNT; i++) {
		buf[i].dwFlags = 0;
		buf[i].dwBufferLength = BUFFER_SIZE * 2;
		buf[i].lpData = malloc(BUFFER_SIZE * 2);
		memset(buf[i].lpData, 0, BUFFER_SIZE * 2);
		waveOutPrepareHeader(hwo, buf + i, sizeof(WAVEHDR));
	}

	while (flagRunSound) {
		

		//if (GetAsyncKeyState(0x30) < 0)
		//	sounds[0].isused = 1;
		//if (GetAsyncKeyState(0x31) < 0)
		//	sounds[1].isused = 1;
		//if (GetAsyncKeyState(0x32) < 0)
		//	sounds[2].isused = 1;
		//if (GetAsyncKeyState(0x33) < 0)
		//	sounds[3].isused = 1;
		//if (GetAsyncKeyState(0x34) < 0)
		//	sounds[4].isused = 1;
		//if (GetAsyncKeyState(0x35) < 0)
		//	sounds[5].isused = 1;

		// waiting for sound to be played
		while (buf[bufIdx].dwFlags & WHDR_INQUEUE)
			Sleep(1);

		short *const p = (short*)buf[bufIdx].lpData;
		memset(p, 0, BUFFER_SIZE * 2);



		for (int i = 0; i < BUFFER_SIZE; i++) {
			p[i] += backgroundMusic.wavdata[backgroundMusic.wavPos] - 127;
			backgroundMusic.wavPos = (backgroundMusic.wavPos + 1) % backgroundMusic.wavLength;

		}



		for (int j = 0; j < 100; j++) {
			if (sounds[j].isused >= 1) {
				for (int i = 0; i < BUFFER_SIZE; i++) {
					p[i] += sounds[j].wavdata[sounds[j].wavPos] - 127;
					if (sounds[j].wavPos >= sounds[j].wavLength) {
						sounds[j].isused--;
						sounds[j].wavPos = 0;
						break;
					}
					sounds[j].wavPos++;
					//sounds[j].wavPos = (sounds[j].wavPos + 1) % sounds[j].wavLength;

				}
			}
		}

		// adjust volume
		for (int i = 0; i < BUFFER_SIZE; i++)
			p[i] *= 64;

		waveOutWrite(hwo, &buf[bufIdx], sizeof(WAVEHDR));
		bufIdx = (bufIdx + 1) % BUFFER_COUNT;
	}
}