#include "shaderBaseClass.h"

ShaderBaseClass::ShaderBaseClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}

ShaderBaseClass::ShaderBaseClass(const ShaderBaseClass& other)
{
}

ShaderBaseClass::~ShaderBaseClass()
{
}

void ShaderBaseClass::Shutdown()
{
	ShutdownShader();
}

bool ShaderBaseClass::Render(ID3D11DeviceContext* devCon, int indexCount)
{
	bool result;
	result = SetShaderParameters(devCon);
	if(!result)
	{
		return false;
	}

	RenderShader(devCon, indexCount);
	
	return true;
}

void ShaderBaseClass::ShutdownShader()
{
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}
	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	
	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}
}

void ShaderBaseClass::RenderShader(ID3D11DeviceContext* devCon, int indexCount)
{
	devCon->IASetInputLayout(m_layout);

	devCon->PSSetShader(m_pixelShader, NULL, 0);
	devCon->VSSetShader(m_vertexShader, NULL, 0);
	
	devCon->PSSetSamplers(0, 1, &m_sampleState);
	
	devCon->DrawIndexed(indexCount, 0, 0);
}

void ShaderBaseClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

D3DXMATRIX* ShaderBaseClass::PopMatrix()
{
	D3DXMATRIX* mat;
	mat = m_stMatrix.top();
	m_stMatrix.pop();
	return mat;
}

D3DXVECTOR3* ShaderBaseClass::PopVec3()
{
	D3DXVECTOR3* vec;
	vec = m_stVec3.top();
	m_stVec3.pop();
	return vec;
}

float ShaderBaseClass::PopFloat()
{
	float value;
	value = m_stFloat.top();
	m_stFloat.pop();
	return value;
}

ID3D11ShaderResourceView* ShaderBaseClass::PopResourceView()
{
	ID3D11ShaderResourceView* rv;
	rv = m_stResourceView.top();
	m_stResourceView.pop();
	return rv;
}

void ShaderBaseClass::PushFloat(float value)
{
	m_stFloat.push(value);
}

void ShaderBaseClass::PushMatrix(D3DXMATRIX* mat)
{
	m_stMatrix.push(mat);
}

void ShaderBaseClass::PushVec3(D3DXVECTOR3* vec)
{
	m_stVec3.push(vec);
}

void ShaderBaseClass::PushResourceView(ID3D11ShaderResourceView* rv)
{
	m_stResourceView.push(rv);
}