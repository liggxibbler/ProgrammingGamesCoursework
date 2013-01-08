#ifndef _ALPHAFADESHADER_CLASS_H_
#define _ALPHAFADESHADER_CLASS_H_

// INCLUDES
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h> // what's this for?
#include <fstream>
using namespace std;

class AlphaFadeShaderClass
{
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
	struct AlphaBufferType
	{
		float alpha;
		D3DXVECTOR3 padding;
	};
public:
	AlphaFadeShaderClass();
	AlphaFadeShaderClass(const AlphaFadeShaderClass&);
	~AlphaFadeShaderClass();

	bool Initialize(ID3D11Device* device, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* devCon, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView** texture,
		float alpha);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext* devCon, D3DXMATRIX worldMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix, ID3D11ShaderResourceView** texture,
		float alpha);
	void RenderShader(ID3D11DeviceContext* devCon, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_alphaBuffer;
};

#endif