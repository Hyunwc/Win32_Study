#include "4Myun.h"

void load4Myun()
{
}

void free4Myun()
{
}

glm::vec4 eye(0.0f, 0.0f, 1.0f, 0.0f);
glm::vec4 center(0, 0, 0, 0);
glm::vec4 up(0, 1, 0, 0);

void draw4Myun(float dt)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(-1, 1, -1, 1, -2, 2);
	glm::mat4 m = glm::ortho(-1, 1, -1, 1, -2, 10);
	glLoadMatrixf((float*)&m); // 반영

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(eye[0], eye[1], eye[2], 
	//	center[0], center[1], center[2],
	//	up[0], up[1], up[2]);
	m = glm::lookAt(
		glm::vec3(eye[0], eye[1], eye[2]),
		glm::vec3(center[0], center[1], center[2]),
		glm::vec3(up[0], up[1], up[2]));
	glLoadMatrixf((float*)&m); // 반영

	// 1바퀴 돌리고 eye(보는 방향)에 곱한다
	m = glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0, 1, 0));
	eye = m * eye;

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// coord + color
	float vtx[] = {
		0.0f, 0.5f, 0.0f,  	  1.0f, 0.0f, 0.0f, 1.0f,
 		-0.5f, -0.5f, 0.0f,	  0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	  0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.5f, 	  1.0f, 1.0f, 1.0f, 1.0f,
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(float) * 7, &vtx[0]);
	glColorPointer(4, GL_FLOAT, sizeof(float) * 7, &vtx[3]);

	// 시계 반대 방향
	uint8 indices[] = { 
		3, 1, 2, 
		3, 2, 0,
		3, 0, 1,
		0, 2, 1,
	};

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void key4Myun(iKeyStat stat, iPoint point)
{
}
