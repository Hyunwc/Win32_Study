
#include "PS.h"

ParticleSystem::ParticleSystem()
{
	particleMax = 1000;
	_particle = new Particle[particleMax];
	memset(_particle, 0x00, sizeof(Particle) * particleMax);
	particle = new Particle * [particleMax];
	particleNum = 0;

	_createDt = 0.1f;
	createDt = 0.0f; 

	life = 0.5f, lifeVar = 0.4f; // 0.2 ~ 0.3
	v = 10.0f, vVar = 90.0f; // 0.8 ~ 1.2
	ss = 1, ssVar = 9.0f; // ss 1 ~ 10
	se = 1, seVar = 2.0f; // s 1 ~ 3
	//cs = iColor4fWhite, csVar = iColor4fMake(0, 0, 0, 0);
	cs = iColor4fWhite, csVar = iColor4fMake(0, 0, 0, 0);
	//ce = iColor4fMake(1, 0.3, 1, 1), ceVar = iColor4fMake(0, 0, 0, 0);
	ce = iColor4fBlack, ceVar = iColor4fMake(0, 0, 0, 0);
}

ParticleSystem::ParticleSystem(const char* szFormat, ...)
{
	load(szFormat);
	// particleMax From file.....
	_particle = new Particle[particleMax];
	memset(_particle, 0x00, sizeof(Particle) * particleMax);
	particle = new Particle * [particleMax];
	particleNum = 0;

	createDt = 0.0f;
}

ParticleSystem::~ParticleSystem()
{
	delete _particle;
	delete particle;
}

void ParticleSystem::paint(float dt, iPoint point)
{
	// draw
	for (int i = 0; i < particleNum; i++)
	{
		if (particle[i]->paint(dt))
		{
			particleNum--;
			particle[i] = particle[particleNum];
			i--;
		}
	}
	
	//입자 생성
	createDt += dt;
	for (; createDt >= _createDt; createDt -= _createDt)
	{
		Particle* p = NULL;
		for (int i = 0; i < particleMax; i++)
		{
			Particle* t = &_particle[i];
			if (t->life < t->_life)
				continue;
			p = t;
			break;
		}
		// p 초기화
		initParticle(p, point);
		particle[particleNum] = p;
		particleNum++;
	}
}

void ParticleSystem::initParticle(Particle* p, iPoint pos)
{
	p->_life = life + lifeVar * ((rand() % 100) / 100.0f);
	p->life = 0.0f;

	p->p = pos + iPointMake(-5 + rand() % 10, -2 + rand() % 4);
	p->v = iPointMake(0, -1) * // iPointRotate 75 ~ 105
		(v + vVar * ((rand() % 100) / 100.0f)); // 10 ~ 90
	p->ss = ss + ssVar * ((rand() % 100) / 100.0f);
	p->se = se + seVar * ((rand() % 100) / 100.0f);
	p->cs = cs + csVar * ((rand() % 100) / 100.0f);
	p->ce = ce + ceVar * ((rand() % 100) / 100.0f);
}

bool Particle::paint(float dt)
{
	// draw
	float r0 = life / _life;
	float r1 = 1.0f - r0;

	float size = ss * r1 + se * r0;
	iColor4f c = cs * r1 + ce * r0;
	setRGBA(c.r, c.g, c.b, c.a);

	float half = size / 2;
	fillRect(iRectMake(p.x - half, p.y - half, size, size));

	// ctrl
	p += v * dt;

	life += dt;

	return life >= _life;
}

void ParticleSystem::start()
{
}

void ParticleSystem::stop()
{
}

void ParticleSystem::save(const char* path)
{
	// 파일의 경로와, 모드(쓰기(binary형태로))
	FILE* pf = fopen(path, "wb");

	fwrite(&particleMax, sizeof(int), 1, pf);
	fwrite(&_createDt, sizeof(float), 1, pf);
	fwrite(&life, sizeof(float), 1, pf);
	fwrite(&lifeVar, sizeof(float), 1, pf);

	// not used..
	fwrite(&v, sizeof(int), 1, pf);
	fwrite(&vVar, sizeof(int), 1, pf);

	fwrite(&ss, sizeof(float), 1, pf);
	fwrite(&ssVar, sizeof(float), 1, pf);
	fwrite(&se, sizeof(float), 1, pf);
	fwrite(&seVar, sizeof(float), 1, pf);

	fwrite(&cs, sizeof(iColor4f), 1, pf);
	fwrite(&csVar, sizeof(iColor4f), 1, pf);
	fwrite(&ce, sizeof(iColor4f), 1, pf);
	fwrite(&ceVar, sizeof(iColor4f), 1, pf);

	fclose(pf);
}

void ParticleSystem::load(const char* path)
{
	// 파일의 경로와, 모드(읽기binary형태로))
	FILE* pf = fopen(path, "rb");

	fread(&particleMax, sizeof(int), 1, pf);
	fread(&_createDt, sizeof(float), 1, pf);
	fread(&life, sizeof(float), 1, pf);
	fread(&lifeVar, sizeof(float), 1, pf);

	// not used..
	fread(&v, sizeof(int), 1, pf);
	fread(&vVar, sizeof(int), 1, pf);
	
	fread(&ss, sizeof(float), 1, pf);
	fread(&ssVar, sizeof(float), 1, pf);
	fread(&se, sizeof(float), 1, pf);
	fread(&seVar, sizeof(float), 1, pf);
	 
	fread(&cs, sizeof(iColor4f), 1, pf);
	fread(&csVar, sizeof(iColor4f), 1, pf);
	fread(&ce, sizeof(iColor4f), 1, pf);
	fread(&ceVar, sizeof(iColor4f), 1, pf);

	fclose(pf);
}

void ParticleSystem::move(iKeyStat stat, iPoint point)
{
}
