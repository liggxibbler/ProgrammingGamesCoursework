////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

enum ModelEnum
{
	MODEL_FROM_FILE = 0x00,
	MODEL_CYLINDER  = 0x01
};

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tangent;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float bx, by, bz;
		float tx, ty, tz;

		ModelType()
		{
			x = 0;
			y = 0;
			z = 0;

			tu = 0;
			tv = 0;

			nx = 0;
			ny = 0;
			nz = 0;

			bx = 0;
			by = 0;
			bz = 0;

			tx = 0;
			ty = 0;
			tz = 0;
		}
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*);
	bool Initialize(ID3D11Device*, float, float, int, int, float, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*, D3DXMATRIX& worldMatrix);

	bool CalculateBNT(ID3D11Device* device);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture(int index);
	ID3D11ShaderResourceView** GetTexture();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	bool LoadCylinder(float Radius, float Height, int numRSlice, int numVSlice, float texScale);
	void ReleaseModel();
	
	void CalculateTangentBinormal(int, int, int);
	void CalculateNormal(int, int, int);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_model;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
};

#endif