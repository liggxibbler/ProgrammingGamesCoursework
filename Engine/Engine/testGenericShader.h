#include "shaderBaseClass.h"

class TestGenericShader : public ShaderBaseClass
{
public:
	virtual bool Initialize(ID3D11Device*, HWND);
private:
	virtual bool SetShaderParameters(ID3D11DeviceContext*);
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
};