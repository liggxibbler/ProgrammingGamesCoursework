#ifndef _PARTICLE_CLASS_H_
#define _PARTICLE_CLASS_H_

#include "modelclass.h"

class ParticleClass
{
public:
	ParticleClass();
	ParticleClass(ParticleClass&);
	~ParticleClass();

	bool Initialize(WCHAR*, int);//texture, number of quads, - maybe lifespan and movement pattern
	void Frame();
	void Render();
	void Shutdown();

private:
	// sort method
	void SortByZ(D3DXMATRIX& viewMatrix);
private:
	ModelClass* m_quad;
	int m_quadCount;
};

#endif