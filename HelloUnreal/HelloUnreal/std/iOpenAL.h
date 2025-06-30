#pragma once

#include "iDefine.h"

#include "al.h"
#include "alc.h"

#pragma comment(lib, "OpenAL32")

class iOpenAL
{
public:
	iOpenAL(int count);
	virtual ~iOpenAL();

	void initBuffer(int index, const char* szFormat, ...);
	void initSource(int idx, bool repeat);

	// 트랙중에 몇번째 것을
	void play(int index);
	void stop(int index = -1);
	void pause(int index = -1);
	void volume(int index, float vol);

	// unsigned int
	ALuint* gBuffer;
	ALuint* gSource;
	int bufCount;
};

struct WaveHeader0
{
	char riff[4];			// "RIFF"
	unsigned int riffSize;
	char wave[4];			// "WAVE"
	
	char fmt[4];			// "fmt "
	unsigned int fmtSize;	// 
	unsigned short format;	// 2byte
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int bytesPerSec;
	unsigned short blockAlign;
	unsigned short bitPerSample;
};

struct WaveHeader1
{
	char data[4];
	unsigned int dataSize;
};