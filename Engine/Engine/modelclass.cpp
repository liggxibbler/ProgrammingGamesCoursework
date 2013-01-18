////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, WCHAR* normalFilename)
{
	bool result;


	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename, normalFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::Initialize(ID3D11Device* device, float R, float H, int r, int v, float ts, WCHAR* filename, WCHAR* normalmap)
{
	bool result;


	// Load in the model data,
	result = LoadCylinder(R, H, r, v, ts);
	if(!result)
	{
		return false;
	}


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	//CalculateBNT(device);

	// Load the texture for this model.
	result = LoadTexture(device, filename, normalmap);
	if(!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX& worldMatrix)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture(int index)
{
	return m_Texture->GetTexture(index);
}

ID3D11ShaderResourceView** ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].binormal = D3DXVECTOR3(m_model[i].bx, m_model[i].by, m_model[i].bz);
		vertices[i].tangent = D3DXVECTOR3(m_model[i].tx, m_model[i].ty, m_model[i].tz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename, WCHAR* normalmap)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename, normalmap);
	if(!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* heightmap, WCHAR* primary, WCHAR* secondary, WCHAR* tertiary)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, heightmap, primary, secondary, tertiary);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}


void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}

bool ModelClass::LoadCylinder(float Radius, float Height, int numRSlice, int numVSlice, float texScale)
{
	m_vertexCount = 6 * ((numRSlice+1) * (numVSlice+1)) + 6 * (numRSlice);
	m_indexCount = m_vertexCount;
	
	ModelType** side = new ModelType*[numVSlice+1];
	if(!side)
	{
		return false;
	}
	ModelType* top = new ModelType[numRSlice + 1];
	if(!top)
	{
		return false;
	}
	ModelType* bottom = new ModelType[numRSlice + 1];
	if(!bottom)
	{
		return false;
	}

	int index = 0;
	float ang, hAct;
	float x, z;
	float u, v;
	int h1, a1;

	// Create temporary points for the side

	for (int h = 0; h <= numVSlice; h++)
	{
		side[h] = new ModelType[numRSlice+1];
		for (int a = 0; a <= numRSlice; a++)
		{
			ang = (float)a * 2 * D3DX_PI / numRSlice;
			hAct = h * Height / numVSlice;
			side[h][a].x = Radius * cos(ang);
			side[h][a].y = hAct;
			side[h][a].z = Radius * sin(ang);
			side[h][a].tu = ang / (2*D3DX_PI) * texScale;
			side[h][a].tv = (Height - hAct) / Height * texScale;
			side[h][a].nx = cos(ang);
			side[h][a].ny = 0;
			side[h][a].nz = sin(ang);
			side[h][a].bx = -sin(ang);
			side[h][a].by = 0;
			side[h][a].bz = cos(ang);
			side[h][a].tx = 0;
			side[h][a].ty = -1;
			side[h][a].tx = 0;
		}
	}
	
	// Create temporary points on the top and bottom caps

	for (int a = 0; a < numRSlice; a++)
	{
		ang = (float)a * 2 * D3DX_PI / numRSlice;
		x = Radius * cos(ang);
		z = Radius * sin(ang);
		u = (cos(ang) + 1) * .5f;
		v = (sin(ang) + 1) * .5f;
		
		top[index].x = x;
		top[index].y = Height;
		top[index].z = z;
		top[index].tu = u * texScale;
		top[index].tv = v * texScale;
		top[index].bx = 1.0f;
		top[index].by = 0.0f;
		top[index].bz = 0.0f;
		top[index].tx = 0.0f;
		top[index].ty = 0.0f;
		top[index].tz = 1.0f;

		bottom[index].x = x;
		bottom[index].y = 0;
		bottom[index].z = z;
		bottom[index].tu = u * texScale;
		bottom[index++].tv = v * texScale;
		bottom[index].bx = 0.0f;
		bottom[index].by = 0.0f;
		bottom[index].bz = 1.0f;
		bottom[index].tx = 1.0f;
		bottom[index].ty = 0.0f;
		bottom[index].tz = 0.0f;
	}

	top[index].x = 0;
	top[index].y = Height;
	top[index].z = 0;

	top[index].tu = .5f * texScale;
	top[index].tv = .5f * texScale;

	bottom[index].x = 0;
	bottom[index].y = 0;
	bottom[index].z = 0;

	bottom[index].tu = .5f * texScale;
	bottom[index].tv = .5f * texScale;



	// Fill in actual vertices

	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	index = 0;

	for (int h = 0; h < numVSlice; h++)
	{
		for (int a = 0; a < numRSlice; a++)
		{
			h1 = h+1;
			a1 = a+1;
			// first triangle
			m_model[index].x = side[h][a].x;
			m_model[index].y = side[h][a].y - Height * .5f;
			m_model[index].z = side[h][a].z;
			m_model[index].tu = side[h][a].tu;
			m_model[index].tv = side[h][a].tv;
			m_model[index].nx = side[h][a].nx;
			m_model[index].ny = side[h][a].ny;
			m_model[index].nz = side[h][a].nz;
			m_model[index].tx = side[h][a].tx;
			m_model[index].ty = side[h][a].ty;
			m_model[index].tz = side[h][a].tz;
			m_model[index].bx = side[h][a].by;
			m_model[index].by = side[h][a].by;
			m_model[index++].bz = side[h][a].bz;

			m_model[index].x = side[h1][a1].x;
			m_model[index].y = side[h1][a1].y - Height * .5f;
			m_model[index].z = side[h1][a1].z;
			m_model[index].tu = side[h1][a1].tu;
			m_model[index].tv = side[h1][a1].tv;
			m_model[index].nx = side[h1][a1].nx;
			m_model[index].ny = side[h1][a1].ny;
			m_model[index].nz = side[h1][a1].nz;
			m_model[index].tx = side[h1][a1].tx;
			m_model[index].ty = side[h1][a1].ty;
			m_model[index].tz = side[h1][a1].tz;
			m_model[index].bx = side[h1][a1].by;
			m_model[index].by = side[h1][a1].by;
			m_model[index++].bz = side[h1][a1].bz;

			m_model[index].x = side[h][a1].x;
			m_model[index].y = side[h][a1].y - Height * .5f;
			m_model[index].z = side[h][a1].z;
			m_model[index].tu = side[h][a1].tu;
			m_model[index].tv = side[h][a1].tv;
			m_model[index].nx = side[h][a1].nx;
			m_model[index].ny = side[h][a1].ny;
			m_model[index].nz = side[h][a1].nz;
			m_model[index].tx = side[h][a1].tx;
			m_model[index].ty = side[h][a1].ty;
			m_model[index].tz = side[h][a1].tz;
			m_model[index].bx = side[h][a1].by;
			m_model[index].by = side[h][a1].by;
			m_model[index++].bz = side[h][a1].bz;

			// second triangle
			m_model[index].x = side[h][a].x;
			m_model[index].y = side[h][a].y - Height * .5f;
			m_model[index].z = side[h][a].z;
			m_model[index].tu = side[h][a].tu;
			m_model[index].tv = side[h][a].tv;
			m_model[index].nx = side[h][a].nx;
			m_model[index].ny = side[h][a].ny;
			m_model[index].nz = side[h][a].nz;
			m_model[index].tx = side[h][a].tx;
			m_model[index].ty = side[h][a].ty;
			m_model[index].tz = side[h][a].tz;
			m_model[index].bx = side[h][a].by;
			m_model[index].by = side[h][a].by;
			m_model[index++].bz = side[h][a].bz;

			m_model[index].x = side[h1][a].x;
			m_model[index].y = side[h1][a].y - Height * .5f;
			m_model[index].z = side[h1][a].z;
			m_model[index].tu = side[h1][a].tu;
			m_model[index].tv = side[h1][a].tv;
			m_model[index].nx = side[h1][a].nx;
			m_model[index].ny = side[h1][a].ny;
			m_model[index].nz = side[h1][a].nz;
			m_model[index].tx = side[h1][a].tx;
			m_model[index].ty = side[h1][a].ty;
			m_model[index].tz = side[h1][a].tz;
			m_model[index].bx = side[h1][a].by;
			m_model[index].by = side[h1][a].by;
			m_model[index++].bz = side[h1][a].bz;

			m_model[index].x = side[h1][a1].x;
			m_model[index].y = side[h1][a1].y - Height * .5f;
			m_model[index].z = side[h1][a1].z;
			m_model[index].tu = side[h1][a1].tu;
			m_model[index].tv = side[h1][a1].tv;
			m_model[index].nx = side[h1][a1].nx;
			m_model[index].ny = side[h1][a1].ny;
			m_model[index].nz = side[h1][a1].nz;
			m_model[index].tx = side[h1][a1].tx;
			m_model[index].ty = side[h1][a1].ty;
			m_model[index].tz = side[h1][a1].tz;
			m_model[index].bx = side[h1][a1].by;
			m_model[index].by = side[h1][a1].by;
			m_model[index++].bz = side[h1][a1].bz;
		}
	}

	// fill the top and bottom caps

	for(int i=0; i<numRSlice; i++)
	{
		// one from the top	
		m_model[index].x = top[i].x;
		m_model[index].y = top[i].y - Height * .5f;
		m_model[index].z = top[i].z;
		m_model[index].tu = top[i].tu;
		m_model[index].tv = top[i].tv;
		m_model[index].nx = 0.0f;
		m_model[index].ny = 1.0f;
		m_model[index].nz = 0.0f;
		m_model[index].tx = 0.0f;
		m_model[index].ty = 0.0f;
		m_model[index].tz = 1.0f;
		m_model[index].bx = 1.0f;
		m_model[index].by = 0.0f;
		m_model[index++].bz = 0.0f;

		m_model[index].x = top[numRSlice].x;
		m_model[index].y = top[numRSlice].y - Height * .5f;
		m_model[index].z = top[numRSlice].z;
		m_model[index].tu = top[numRSlice].tu;
		m_model[index].tv = top[numRSlice].tv;
		m_model[index].nx = 0.0f;
		m_model[index].ny = 1.0f;
		m_model[index].nz = 0.0f;
		m_model[index].tx = 0.0f;
		m_model[index].ty = 0.0f;
		m_model[index].tz = 1.0f;
		m_model[index].bx = 1.0f;
		m_model[index].by = 0.0f;
		m_model[index++].bz = 0.0f;

		m_model[index].x = top[(i + 1)%numRSlice].x;
		m_model[index].y = top[(i + 1)%numRSlice].y - Height * .5f;
		m_model[index].z = top[(i + 1)%numRSlice].z;
		m_model[index].tu = top[(i + 1)%numRSlice].tu;
		m_model[index].tv = top[(i + 1)%numRSlice].tv;
		m_model[index].nx = 0.0f;
		m_model[index].ny = 1.0f;
		m_model[index].nz = 0.0f;
		m_model[index].tx = 0.0f;
		m_model[index].ty = 0.0f;
		m_model[index].tz = 1.0f;
		m_model[index].bx = 1.0f;
		m_model[index].by = 0.0f;
		m_model[index++].bz = 0.0f;
		
		//one from the bottom
		m_model[index].x = bottom[i].x;
		m_model[index].y = bottom[i].y - Height * .5f;
		m_model[index].z = bottom[i].z;
		m_model[index].tu = bottom[i].tu;
		m_model[index].tv = bottom[i].tv;
		m_model[index].nx = 0.0f;
		m_model[index].ny = -1.0f;
		m_model[index++].nz = 0.0f;

		
		m_model[index].x = bottom[(i + 1)%numRSlice].x;
		m_model[index].y = bottom[(i + 1)%numRSlice].y - Height * .5f;
		m_model[index].z = bottom[(i + 1)%numRSlice].z;
		m_model[index].tu = bottom[(i + 1)%numRSlice].tu;
		m_model[index].tv = bottom[(i + 1)%numRSlice].tv;
		m_model[index].nx = 0.0f;
		m_model[index].ny = -1.0f;
		m_model[index++].nz = 0.0f;

		m_model[index].x = bottom[numRSlice].x;
		m_model[index].y = bottom[numRSlice].y - Height * .5f;
		m_model[index].z = bottom[numRSlice].z;
		m_model[index].tu = bottom[numRSlice].tu;
		m_model[index].tv = bottom[numRSlice].tv;		
		m_model[index].nx = 0.0f;
		m_model[index].ny = -1.0f;
		m_model[index++].nz = 0.0f;
	}


	//if(!CalculateBNT())
	//{
	//	return false;
	//}

	// Clean up
	for(int i=0; i<numVSlice; i++)
	{
		delete [] side[i];
		side[i] = 0;
	}
	delete [] side;
	side = 0;

	delete[] top;
	top = 0;

	delete[] bottom;
	bottom = 0;

	return true;
}

bool ModelClass::CalculateBNT(ID3D11Device* device)
{
	if(!m_vertexBuffer)
	{
		return false;
	}

	for(int i=0; i < m_vertexCount; i+=3)
	{
		CalculateTangentBinormal(i, i+1, i+2);
		CalculateNormal(i, i+1, i+2);
	}

	InitializeBuffers(device);

	return true;
}

void ModelClass::CalculateTangentBinormal(int i1, int i2, int i3)
{
	//  Find two vectors, the tangent an the binormal (or bitangent),
	// aligned with the u and v axes of the texture coordinates of the
	// vertices of each polygon.
	//
	// The equations that are being solved are:
	//   P1 - P0 = (u2 - u1)*B + (v2 - v1)*T
	//   P2 - P0 = (u3 - u1)*B + (v3 - v1)*T
	// for B and T, where P0, P1, and P2 are position vectors,
	// u1,u2,u3 and v1,v2,v3 are their texture coordinates.

	D3DXVECTOR3 P0, P1, P2;
	D3DXVECTOR3 Q1, Q2, B, T;
	float u11, u12, u21, u22;
	float size;

	P0 = D3DXVECTOR3(m_model[i1].x, m_model[i1].y, m_model[i1].z);
	P1 = D3DXVECTOR3(m_model[i2].x, m_model[i2].y, m_model[i2].z);
	P2 = D3DXVECTOR3(m_model[i3].x, m_model[i3].y, m_model[i3].z);

	Q1 = P1 - P0;
	Q2 = P2 - P0;

	u11 = m_model[i2].tu - m_model[i1].tu;//P1->texture.x - P0->texture.x;
	u12 = m_model[i2].tv - m_model[i1].tv;//P1->texture.y - P0->texture.y;
	u21 = m_model[i3].tu - m_model[i1].tu;//P2->texture.x - P0->texture.x;
	u22 = m_model[i3].tv - m_model[i1].tv;//P2->texture.y - P0->texture.y;

	float detInv = u11*u22 - u21*u12;
	if(detInv == 0)
	//  Equations cannot be solved, probably because the polygon is
	// actually a line segment, i.e. the three vertices lie on the same line
	{
		B = D3DXVECTOR3(0,0,0);
		T = D3DXVECTOR3(0,0,0);
	}
	else
	{
		B = D3DXVECTOR3(u22*Q1 - u12*Q2);
		T = D3DXVECTOR3(-u21*Q1 + u11*Q2);

		size = sqrt(B.x*B.x + B.y*B.y + B.z*B.z);
		B.x = B.x / size;
		B.y = B.y / size;
		B.z = B.z / size;

		size = sqrt(T.x*T.x + T.y*T.y + T.z*T.z);
		T.x = T.x / size;
		T.y = T.y / size;
		T.z = T.z / size;
	}

	m_model[i1].tx = T.x;
	m_model[i1].ty = T.y;
	m_model[i1].tz = T.z;

	m_model[i2].tx = T.x;
	m_model[i2].ty = T.y;
	m_model[i2].tz = T.z;

	m_model[i3].tx = T.x;
	m_model[i3].ty = T.y;
	m_model[i3].tz = T.z;

	m_model[i1].bx = B.x;
	m_model[i1].by = B.y;
	m_model[i1].bz = B.z;

	m_model[i2].bx = B.x;
	m_model[i2].by = B.y;
	m_model[i2].bz = B.z;

	m_model[i3].bx = B.x;
	m_model[i3].by = B.y;
	m_model[i3].bz = B.z;

	return;
}

void ModelClass::CalculateNormal(int i1, int i2, int i3)
{
	D3DXVECTOR3 P1(m_model[i1].x, m_model[i1].y, m_model[i1].z),
		P2(m_model[i2].x, m_model[i2].y, m_model[i2].z),
		P3(m_model[i3].x, m_model[i3].y, m_model[i3].z), Q12, Q13;

	float x, y, z;

	// Find two vectors on the plane of the polygan
	Q12 = P2 - P1;
	Q13 = P3 - P1;

	//  Cross product of Q12 and Q13 to find a direction 
	// perpendicular to the polygon
	x = Q12.y*Q13.z - Q12.z*Q13.y;
	y = Q12.z*Q13.x - Q12.x*Q13.z;
	z = Q12.x*Q13.y - Q12.y*Q13.x;

	// Normalize the normal vector...
	float mag = sqrt(x*x + y*y + z*z);
	x /= mag;
	y /= mag;
	z /= mag;

	m_model[i1].nx = x;
	m_model[i1].ny = y;
	m_model[i1].nz = z;
	
	m_model[i2].nx = x;
	m_model[i2].ny = y;
	m_model[i2].nz = z;

	m_model[i3].nx = x;
	m_model[i3].ny = y;
	m_model[i3].nz = z;

	return;
}

void ModelClass::SetSphericalTexCoords(ID3D11Device* device)
{
	//  Doesn't really work very well, because of the transition between
	// 0 and 2pi not being possible to cover. i.e. the u coordinate goes
	// from 2*pi - delta to zero, which causes an undesirable artefact.

	// u = acos( p . i)
	// v = acos( p . j)

	float x, y, z;
	float x1, y1, z1;
	float mag;
	
	for(int i=0; i<m_vertexCount; i++)
	{
		x = m_model[i].x;
		y = m_model[i].y;
		z = m_model[i].z;

		mag = sqrt(x*x + y*y + z*z);
		
		if(mag!=0)
		{
			y /= mag; // now cosine of angle vertex vector makes with with +y
		}

		m_model[i].tv = acos(y) / D3DX_PI;

		mag = sqrt(x*x + z*z);

		if(mag!=0)
		{
			x /= mag;
		}

		m_model[i].tu = acos(x);
		m_model[i].tu /= 2 * D3DX_PI;

		if(z <= 0)
		{
			m_model[i].tu = 1.0 - m_model[i].tu;
		}
	}

	InitializeBuffers(device);
}

void ModelClass::ScaleTexCoords(float su, float sv, ID3D11Device* device)
{
	for(int i=0;i<m_vertexCount; i++)
	{
		m_model[i].tu *= su;
		m_model[i].tv *= sv;
	}
	InitializeBuffers(device);
}

bool ModelClass::Initialize(ID3D11Device* device, int xCount, int yCount, float uTexScale, float vTexScale,
	WCHAR* heightmap, WCHAR* primary, WCHAR* secondary, WCHAR* tertiary)
{
	bool result;


	// Load in the model data,
	result = LoadQuadMesh(xCount, yCount, uTexScale, vTexScale);
	if(!result)
	{
		return false;
	}


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	//CalculateBNT(device);

	// Load the texture for this model.
	result = LoadTexture(device, heightmap, primary, secondary, tertiary);
	if(!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::LoadQuadMesh(int rows, int cols, float uScale, float vScale)
{
	m_vertexCount = rows * cols * 6;
	m_indexCount = m_vertexCount;

	ModelType** grid = new ModelType*[rows+1];
	if(!grid)
	{
		return false;
	}

	for(int i=0; i<rows+1; i++)
	{
		grid[i] = new ModelType[cols+1];
		if(!grid[i])
		{
			return false;
		}
	}

	float r;

	for(int i=0; i<rows+1;i++)
	{
		for(int j=0; j<cols+1;j++)
		{
			grid[i][j].x = (float)i/(float)rows - .5f;
			grid[i][j].z = 0.5f - (float)j/(float)cols;
			//r = sqrt(grid[i][j].x*grid[i][j].x + grid[i][j].z * grid[i][j].z);
			//grid[i][j].y = sinf(r * 30) * cosf(r * 30) * .05;
			grid[i][j].y = 0;

			grid[i][j].tu = (float)i/(float)rows*uScale;
			grid[i][j].tv = (float)j/(float)cols*vScale;
		}
	}
	
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	int index = 0;

	for(int i=0; i < m_vertexCount; i++)
	{
		m_model[i].x = 0.0f;
		m_model[i].y = 1.0f;
		m_model[i].z = 0.0f;

		m_model[i].bx = 0.0f;
		m_model[i].by = 0.0f;
		m_model[i].bz = -1.0f;

		m_model[i].tx = 1.0f;
		m_model[i].ty = 0.0f;
		m_model[i].tz = 0.0f;
	}

	for(int i=1; i<=cols-1;i++)
	{
		for(int j=0; j<rows-1;j++)
		{
			// first tri

			m_model[index].x = grid[i][j].x;
			m_model[index].y = grid[i][j].y;
			m_model[index].z = grid[i][j].z;
			m_model[index].tu = grid[i][j].tu;
			m_model[index++].tv = grid[i][j].tv;

			m_model[index].x = grid[i-1][j+1].x;
			m_model[index].y = grid[i-1][j+1].y;
			m_model[index].z = grid[i-1][j+1].z;
			m_model[index].tu = grid[i-1][j+1].tu;
			m_model[index++].tv = grid[i-1][j+1].tv;

			m_model[index].x = grid[i-1][j].x;
			m_model[index].y = grid[i-1][j].y;
			m_model[index].z = grid[i-1][j].z;
			m_model[index].tu = grid[i-1][j].tu;
			m_model[index++].tv = grid[i-1][j].tv;

			// second tri

			m_model[index].x = grid[i][j].x;
			m_model[index].y = grid[i][j].y;
			m_model[index].z = grid[i][j].z;
			m_model[index].tu = grid[i][j].tu;
			m_model[index++].tv = grid[i][j].tv;

			m_model[index].x = grid[i][j+1].x;
			m_model[index].y = grid[i][j+1].y;
			m_model[index].z = grid[i][j+1].z;
			m_model[index].tu = grid[i][j+1].tu;
			m_model[index++].tv = grid[i][j+1].tv;

			m_model[index].x = grid[i-1][j+1].x;
			m_model[index].y = grid[i-1][j+1].y;
			m_model[index].z = grid[i-1][j+1].z;
			m_model[index].tu = grid[i-1][j+1].tu;
			m_model[index++].tv = grid[i-1][j+1].tv;
		}
	}

	return true;
}