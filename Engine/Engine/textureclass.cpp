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

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	m_texture = new ID3D11ShaderResourceView*;

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}
	
	return true;
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
			m_texture[1] = 0;
		}
	}
	else
	{
		m_texture[1] = 0;
	}
	return true;
}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* heightmap, WCHAR* primary, WCHAR* secondary, WCHAR* tertiary)
{
	HRESULT result;

	m_texture = new ID3D11ShaderResourceView*[4];

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, heightmap, NULL, NULL, &(m_texture[0]), NULL);
	if(FAILED(result))
	{
		return false;
	}
	
	if(primary != NULL)
	{
		result = D3DX11CreateShaderResourceViewFromFile(device, primary, NULL, NULL, &(m_texture[1]), NULL);
		if(FAILED(result))
		{
			return false;
		}
	}
	else
	{
		m_texture[1] = 0;
	}

	if(secondary != NULL)
	{
		result = D3DX11CreateShaderResourceViewFromFile(device, primary, NULL, NULL, &(m_texture[2]), NULL);
		if(FAILED(result))
		{
			return false;
		}
	}
	else
	{
		m_texture[2] = 0;
	}

	if(tertiary != NULL)
	{
		result = D3DX11CreateShaderResourceViewFromFile(device, tertiary, NULL, NULL, &(m_texture[3]), NULL);
		if(FAILED(result))
		{
			return false;
		}
	}
	else
	{
		m_texture[3] = 0;
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