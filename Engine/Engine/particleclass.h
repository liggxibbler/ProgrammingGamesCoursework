#ifndef _PARTICLE_CLASS_H_
#define _PARTICLE_CLASS_H_

#include "modelclass.h"

class ParticleClass
{
public:
	ParticleClass();
	ParticleClass(ParticleClass&);
	~ParticleClass();

	bool Initialize(int count, float* ranges, bool random); // random or uniform distribution
	void Shutdown();
	void SortByZ(D3DXMATRIX &viewMatrix);
private:
	int m_particleCount;
	float* m_position;
	float** m_params;
};

#endif