#pragma once

#include "iDefine.h"

struct STInfo
{
	const char* pathVert;
	const char* pathFrag[5];
	// programID[5]
	Texture* tex[5][4];
	int buf[5][4];
};

extern STInfo stInfo;

class iShadertoy
{
public:
	iShadertoy(STInfo* info);
	virtual ~iShadertoy();

private:
	void setUniform(float dt, uint32 programID);

public:
	void paint(float dt);
	void key(iKeyStat stat, iPoint point);

	uint32* programID;

	//Texture* texs[4][2];
	// 백버퍼4개 2개가 한쌍
	Texture*** texs; // 백버퍼 버퍼A~D가 그리는 도화지
	Texture*** texiChannel; // Texture* iChannel[5][4]
	int** bufiChannel; // int bufiChannel[5][4]
	bool toggle;

	float iTime;
	int iFrame;
	float* iChannelTime;
	float* iMouse;
};

#define uniformMat(id, x)			glUniformMatrix4fv(glGetUniformLocation(programID, id), 1, false, (float*)&x)
#define uniform1fv(id, num, x)		glUniform1fv(glGetUniformLocation(programID, id), num, x)
#define uniform3fv(id, num, v)	    glUniform3fv(glGetUniformLocation(programID, id), num, v);

#define uniform1f(id, x)			glUniform1f(glGetUniformLocation(programID, id), x)
#define uniform2f(id, x, y)			glUniform2f(glGetUniformLocation(programID, id), x, y)
#define uniform3f(id, x, y, z)		glUniform3f(glGetUniformLocation(programID, id), x, y, z)
#define uniform4f(id, x, y, z, w)	glUniform4f(glGetUniformLocation(programID, id), x, y, z, w)
#define uniform1i(id, x)			glUniform1i(glGetUniformLocation(programID, id), x)
#define uniform2i(id, x, y)			glUniform2i(glGetUniformLocation(programID, id), x, y)
#define uniform3i(id, x, y, z)		glUniform3i(glGetUniformLocation(programID, id), x, y, z)
#define uniform4i(id, x, y, z, w)	glUniform4i(glGetUniformLocation(programID, id), x, y, z, w)
