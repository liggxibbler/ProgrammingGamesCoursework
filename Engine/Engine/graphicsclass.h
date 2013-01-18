////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "particleclass.h"

#include "lightshaderclass.h"
#include "bitmapShaderClass.h"
#include "particleshaderclass.h"
#include "alphaFadeShaderClass.h"
#include "TerrainShaderClass.h"
#include "testGenericShader.h"
#include "fontshaderclass.h"

#include "lightclass.h"

#include "textclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	struct GraphicsUpdateInfo
	{
		bool wKey, aKey, sKey, dKey;
		int mouseDiffX, mouseDiffY;
		float time;
		int fps, cpu;
	};

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(GraphicsUpdateInfo&);

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	
	ModelClass* m_Groundmesh;
	ModelClass* m_Ground2;
	ModelClass* m_Billboard;
	ModelClass* m_Sphere;
	
	//ModelClass* m_bbFire;
	ModelClass* m_bbClouds;
	ModelClass* m_bbRain;
	ModelClass* m_bbSnow;
	ModelClass* m_bbTrees;

	ParticleClass* m_fire;
	ParticleClass* m_clouds;
	ParticleClass* m_rain;
	ParticleClass* m_snow;
	ParticleClass* m_trees;

	LightShaderClass* m_LightShader;
	BitmapShaderClass* m_BitmapShader;
	ParticleShaderClass* m_ParticleShader;
	AlphaFadeShaderClass* m_alphaFadeShader;
	TestGenericShader* m_testShader;
	TerrainShaderClass* m_terrainShader;
	FontShaderClass* m_FontShader;

	LightClass* m_Light;
	TextClass* m_Text;

	D3DXVECTOR3* m_positions;
	float* m_phases;
	float* m_speeds;
};

#endif