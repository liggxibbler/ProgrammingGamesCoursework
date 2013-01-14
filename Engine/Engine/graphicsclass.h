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
#include "lightshaderclass.h"
#include "bitmapShaderClass.h"
#include "particleshaderclass.h"
#include "alphaFadeShaderClass.h"
#include "testGenericShader.h"
#include "lightclass.h"
#include "timerclass.h"


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
	ModelClass* m_Model;
	ModelClass* m_Model2;
	ModelClass* m_Sphere;
	LightShaderClass* m_LightShader;
	BitmapShaderClass* m_BitmapShader;
	ParticleShaderClass* m_ParticleShader;
	AlphaFadeShaderClass* m_alphaFadeShader;
	TestGenericShader* m_testShader;
	LightClass* m_Light;
	D3DXVECTOR3* m_positions;
	float* m_phases;
	float* m_speeds;
};

#endif