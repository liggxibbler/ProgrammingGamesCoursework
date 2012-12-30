////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"


TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename, WCHAR* filename2)
{
	HRESULT result;

	m_texture = new ID3D11ShaderResourceView*[2];

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &(m_texture[0]), NULL);
	if(FAILED(result))
	{
		return false;
	}
	
	if(filename2 != NULL)
	{
		result = D3DX11CreateShaderResourceViewFromFile(device, filename2, NULL, NULL, &(m_texture[1]), NULL);
		if(FAILED(result))
		{
			return false;
		}
	}
	else
	{
		m_texture[1] = 0;
	}
	return true;
}


void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture[0])
	{
		m_texture[0]->Release();
		m_texture[0] = 0;
	}

	if(m_texture[1])
	{
		m_texture[1]->Release();
		m_texture[1] = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture(int index)
{
	return m_texture[index];
}

ID3D11ShaderResourceView** TextureClass::GetTexture()
{
	return m_texture;
}