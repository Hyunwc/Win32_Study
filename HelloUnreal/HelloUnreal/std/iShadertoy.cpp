#include "iShadertoy.h"

#include "iStd.h"


iShadertoy::iShadertoy()
{
	programID = new uint32[5];

	texs = new Texture * *[4];
	for (int i = 0; i < 4; i++)
	{
		texs[i] = new Texture * [2];
		memset(texs, 0x00, sizeof(Texture*) * 2);
	}
	
	toggle = false;

	iTime = 0.0f;
	iFrame = 0;
	iChannelTime = new float[4];
	memset(iChannelTime, 0x00, sizeof(float) * 4);
	iMouse = new float[4];
	memset(iMouse, 0x00, sizeof(float) * 4);
}

iShadertoy::~iShadertoy()
{
	for (int i = 0; i < 5; i++)
		iShader::deleteProgram(programID[i]);
	delete programID;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (texs[i][j])
				freeImage(texs[i][j]);
		}
		delete texs[i];
	}

	delete texs;

	delete iChannelTime;
	delete iMouse;
}

void iShadertoy::setUniform(float dt, uint32 programID)
{
	uniform3f("iResolution", devSize.width, devSize.height, 0);
	uniform1f("iTime", iTime); iTime += dt;
	uniform1f("iTimeDelta", dt);
	uniform1f("iFrameRate", 0);
	uniform1i("iFrame", iFrame); iFrame++;
	
#if 1
	uniform1fv("iChannelTime", 4, iChannelTime);
#elif 0
	uniform4f("iChannelTime", iChannelTime[0], iChannelTime[1], iChannelTime[2], iChannelTime[3]);
#else
	for (int i = 0; i < 4; i++)
	{
		char s[16];
		sprintf(s, "iChannelTime[%d]", i);
		uniform1f("s", iChannelTime[i]);
	}
#endif
	float v[12] = {
		texs[0][0]->width, texs[0][0]->height, 0,
		texs[1][0]->width, texs[1][0]->height, 0,
		texs[2][0]->width, texs[2][0]->height, 0,
		texs[3][0]->width, texs[3][0]->height, 0,
	};
	uniform3fv("iChannelResolution", 12, v);
	uniform4f("iMosue", iMouse[0], iMouse[1], iMouse[2], iMouse[3]);
}


void iShadertoy::paint(float dt)
{
	// 4까지는 백버퍼
	for (int i = 0; i < 5; i++)
	{
		if (i < 4)
		{
			fbo->bind(texs[i][toggle]);
		}

		uint32 programID = this->programID[i];
		glUseProgram(programID); // programID를 사용할게

		glm::mat4 projMatrix = glm::ortho(0.0f, devSize.width, devSize.height, 0.0f, -1000.0f, 1000.0f);
		uniformMat("projMatrix", projMatrix);
		glm::mat4 viewMatrix(1.0f);
		uniformMat("viewMatrix", viewMatrix);

		setUniform(dt, programID); // shadertoy uniform setting

		// 포지션의 정점 데이터를 vbo에게 넘기고
		float position[] = {
			0, 0, 0, 1,					devSize.width, 0, 0, 1,
			0, devSize.height, 0, 1,	devSize.width, devSize.height, 0, 1
		};

		// gpu가 사용할 메모리공간(vbo) 할당
		glBindBuffer(GL_ARRAY_BUFFER, vbo);// 1
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, position);

		uint32 pAttr = glGetAttribLocation(programID, "position");
		glEnableVertexAttribArray(pAttr);// 2
		glVertexAttribPointer(pAttr, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 0));

		for (int j = 0; j < 4; j++)
		{
			char s[16];
			sprintf(s, "iChannel%d", i);
			glUniform1i(glGetUniformLocation(programID, s), i);
			glActiveTexture(GL_TEXTURE0 + i);
			Texture* tex = NULL; // 임시
			glBindTexture(GL_TEXTURE_2D, tex->texID);
		}
		
		// TODO : 여기에 그리는 코드를 추가합니다

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);// 3
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// 3

		glDisableVertexAttribArray(pAttr);// 2
		glBindBuffer(GL_ARRAY_BUFFER, 0);// 1

		if (i < 4)
		{
			fbo->unbind();
		}
	}

	toggle = !toggle;
}

void iShadertoy::key(iKeyStat stat, iPoint point)
{
	switch (stat)
	{
	case iKeyStatBegan:
		//iMouse[0] = point.x;
		//iMouse[1] = point.y;
		memcpy(iMouse, &point, sizeof(iPoint));
		iMouse[2] = 1;
		break;
	case iKeyStatMoved:
		if (iMouse[2])
		{
			//iMouse[0] = point.x;
			//iMouse[1] = point.y;
			memcpy(iMouse, &point, sizeof(iPoint));
		}
		break;
	case iKeyStatEnded:
		iMouse[2] = 0;
		break;
	}
}



