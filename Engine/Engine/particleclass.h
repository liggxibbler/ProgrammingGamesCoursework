#ifndef _PARTICLE_CLASS_H_
#define _PARTICLE_CLASS_H_

#include "modelclass.h"
#include <time.h>

class ParticleClass
{
private:
	struct zSort
	{
		float z;
		int i;
		zSort(float _z, int _i)
		{
			z = _z;
			i = _i;
		}
		zSort()
		{
			i=0;
			z=0.0f;
		}
	};
public:
	ParticleClass();
	ParticleClass(ParticleClass&);
	~ParticleClass();

	bool Initialize(int count, float* ranges, float* paramRanges, float life, bool random, int type); // random or uniform distribution
	void Shutdown();
	void SortByZ(D3DXMATRIX &viewMatrix);
	int GetType();
	float* GetSpeeds();
	float* GetPhases();
	float GetLife();
	int GetCount();
	int GetIndex(int index);
	D3DXVECTOR3* GetPositions();

private:
	int m_particleCount;
	int m_type;
	D3DXVECTOR3* m_positions;
	float* m_ranges;
	float* m_phases;
	float* m_speeds;
	zSort* m_zPos;
	float m_life;
};

#endif