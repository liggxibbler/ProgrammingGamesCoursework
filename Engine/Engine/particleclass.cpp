#include "particleclass.h"

ParticleClass::ParticleClass()
{
	m_positions = 0;
	m_phases = 0;
	m_speeds = 0;
	m_life = 0;
}

ParticleClass::ParticleClass(ParticleClass&)
{
}

ParticleClass::~ParticleClass()
{
}

bool ParticleClass::Initialize(int count, float* ranges, float* paramRanges, float life, bool random, int type) // random or uniform distribution
{
	srand(time(NULL) * 1000.0f);

	m_particleCount = count;
	m_ranges = ranges;
	
	m_positions = new D3DXVECTOR3[m_particleCount];
	m_speeds = new float[m_particleCount];
	m_phases = new float[m_particleCount];
	m_life = life;

	float x, y, z;
	float dx, dy, dz; // used to setup the 
	float dp1, dp2;
	int spread; // determine how wide apart the values are
	float fSpread;

	spread = m_particleCount;
	fSpread = static_cast<float>(m_particleCount);

	dx = ranges[1] - ranges[0];
	dy = ranges[3] - ranges[2];
	dz = ranges[5] - ranges[4];

	dp1 = paramRanges[1] - paramRanges[0];
	dp2 = paramRanges[3] - paramRanges[2];
	
	for(int i =0; i<m_particleCount; i++)
	{
		x = ranges[0] + dx * (rand()%spread)/fSpread;
		y = ranges[2] + dy * (rand()%spread)/fSpread;
		z = ranges[4] + dz * (rand()%spread)/fSpread;
		m_positions[i] = D3DXVECTOR3(x,y,z);

		x = paramRanges[0] + dp1 * (rand()%spread)/fSpread;
		y = paramRanges[2] + dp2 * (rand()%spread)/fSpread;
		m_phases[i] = x;
		m_speeds[i] = y;
	}

	 m_zPos = new zSort[m_particleCount];

	return true;
}

void ParticleClass::Shutdown()
{
	if(m_positions)
	{
		delete [] m_positions;
		m_positions = 0;
	}

	if(m_speeds)
	{
		delete [] m_speeds;
		m_speeds = 0;
	}

	if(m_phases)
	{
		delete [] m_phases;
		m_phases = 0;
	}

}

void ParticleClass::SortByZ(D3DXMATRIX &viewMatrix)
{
	zSort zTemp;
	float zi, zj;
	D3DXVECTOR3 tempVec;
	
	for(int i =0; i<m_particleCount; i++)
	{
		D3DXVec3TransformCoord(&tempVec, &m_positions[i], &viewMatrix);
		m_zPos[i].z = tempVec.z;
		m_zPos[i].i = i;
	}
	// sort positions by z value, using the bubble sort algorithm
	for(int i=0;i<m_particleCount;i++)
	{
		for(int j = i; j<m_particleCount; j++)
		{
			zi = m_zPos[i].z;
			zj = m_zPos[j].z;
			if(zi < zj)
			{
				zTemp = zSort(m_zPos[i].z, m_zPos[i].i);
				m_zPos[i] = zSort(m_zPos[j].z, m_zPos[j].i);
				m_zPos[j] = zSort(zTemp.z, zTemp.i);
			}
		}
	}
}

int ParticleClass::GetType()
{
	return m_type;
}

int ParticleClass::GetIndex(int index)
{
	return m_zPos[index].i;
}

float* ParticleClass::GetSpeeds()
{
	return m_speeds;
}

float* ParticleClass::GetPhases()
{
	return m_phases;
}

float ParticleClass::GetLife()
{
	return m_life;
}

int ParticleClass::GetCount()
{
	return m_particleCount;
}

D3DXVECTOR3* ParticleClass::GetPositions()
{
	return m_positions;
}
