////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderBaseClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERBASECLASS_H_
#define _SHADERBASECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <stack>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderBaseClass
////////////////////////////////////////////////////////////////////////////////
class ShaderBaseClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	ShaderBaseClass();
	ShaderBaseClass(const ShaderBaseClass&);
	~ShaderBaseClass();

	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int);
	void PushMatrix(D3DXMATRIX* matrix);
	void PushResourceView(ID3D11ShaderResourceView* resourceView);

protected:
	virtual bool SetShaderParameters(ID3D11DeviceContext*) = 0;

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	void RenderShader(ID3D11DeviceContext*, int);
	
	D3DXMATRIX* PopMatrix();
	ID3D11ShaderResourceView* PopResourceView();

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState; // Might be common to all, will check later
	ID3D11Buffer* m_matrixBuffer;
	
	stack<D3DXMATRIX*> m_stMatrix;
	stack<ID3D11ShaderResourceView**> m_stResourceView;
};

#endif