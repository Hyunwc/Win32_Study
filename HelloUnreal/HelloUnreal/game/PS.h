#pragma once

#include "iStd.h"

struct Particle
{
	float life, _life; // 생명 주기, 고정 시간
	iPoint p, v; // 위치와 방향
	float ss, se; // 크기
	iColor4f cs, ce; // 시작 색과 끝 색 start, end
	bool paint(float dt);
#if 0
	float as, ae; // v에 대한 가속(acceleration)
	float degree, spin; // spin : 회전축
#endif
};


struct ParticleSystem
{
	// Pool Memory
	Particle* _particle;
	Particle** particle;
	int particleNum, particleMax;
	float createDt, _createDt; // 파티클 생성 주기

	float life, lifeVar; // 0.2 ~ 0.3
	// p
	int v, vVar; // v, v의 변화량
	float ss, ssVar, se, seVar; 
	iColor4f cs, csVar;
	iColor4f ce, ceVar;
#if 0
	float a, aVar; // as 0~1, ae 0~1
	float d, dVar; // 어디서 어디로 회전하겠는가
	iColor4f cs, csVar;
#endif
	ParticleSystem();
	ParticleSystem(const char* szFormat, ...);
	virtual ~ParticleSystem();

	void paint(float dt, iPoint point);
	void initParticle(Particle* p, iPoint pos);
	void start();
	void stop();

	void save(const char* path);
	void load(const char* path);
};
