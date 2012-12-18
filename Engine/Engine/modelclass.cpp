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


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the model data,
	//result = LoadModel(modelFilename);
	//if(!result)
	//{
	//	return false;
	//}

	result = LoadCylinder(1000, .5, 4, 1);
	if(!result)
	{
		return true;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
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


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
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


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
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

bool ModelClass::LoadCylinder(float Radius, float Height, int numRSlice, int numVSlice)
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
			side[h][a].tu = ang / (2*D3DX_PI);
			side[h][a].tv = (Height - hAct) / Height;
			side[h][a].nx = cos(ang);
			side[h][a].ny = 0;
			side[h][a].nz = sin(ang);
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

		top[index].tu = u;
		top[index].tv = v;

		bottom[index].x = x;
		bottom[index].y = 0;
		bottom[index].z = z;

		bottom[index].tu = u;
		bottom[index++].tv = v;
	}

	top[index].x = 0;
	top[index].y = Height;
	top[index].z = 0;

	top[index].tu = .5f;
	top[index].tv = .5f;

	bottom[index].x = 0;
	bottom[index].y = 0;
	bottom[index].z = 0;

	bottom[index].tu = .5f;
	bottom[index].tv = .5f;



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
			m_model[index++].nz = side[h][a].nz;

			m_model[index].x = side[h1][a1].x;
			m_model[index].y = side[h1][a1].y - Height * .5f;
			m_model[index].z = side[h1][a1].z;
			m_model[index].tu = side[h1][a1].tu;
			m_model[index].tv = side[h1][a1].tv;
			m_model[index].nx = side[h1][a1].nx;
			m_model[index].ny = side[h1][a1].ny;
			m_model[index++].nz = side[h1][a1].nz;

			m_model[index].x = side[h][a1].x;
			m_model[index].y = side[h][a1].y - Height * .5f;
			m_model[index].z = side[h][a1].z;
			m_model[index].tu = side[h][a1].tu;
			m_model[index].tv = side[h][a1].tv;
			m_model[index].nx = side[h][a1].nx;
			m_model[index].ny = side[h][a1].ny;
			m_model[index++].nz = side[h][a1].nz;

			// second triangle
			m_model[index].x = side[h][a].x;
			m_model[index].y = side[h][a].y - Height * .5f;
			m_model[index].z = side[h][a].z;
			m_model[index].tu = side[h][a].tu;
			m_model[index].tv = side[h][a].tv;
			m_model[index].nx = side[h][a].nx;
			m_model[index].ny = side[h][a].ny;
			m_model[index++].nz = side[h][a].nz;

			m_model[index].x = side[h1][a].x;
			m_model[index].y = side[h1][a].y - Height * .5f;
			m_model[index].z = side[h1][a].z;
			m_model[index].tu = side[h1][a].tu;
			m_model[index].tv = side[h1][a].tv;
			m_model[index].nx = side[h1][a].nx;
			m_model[index].ny = side[h1][a].ny;
			m_model[index++].nz = side[h1][a].nz;

			m_model[index].x = side[h1][a1].x;
			m_model[index].y = side[h1][a1].y - Height * .5f;
			m_model[index].z = side[h1][a1].z;
			m_model[index].tu = side[h1][a1].tu;
			m_model[index].tv = side[h1][a1].tv;
			m_model[index].nx = side[h1][a1].nx;
			m_model[index].ny = side[h1][a1].ny;
			m_model[index++].nz = side[h1][a1].nz;
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
		m_model[index++].nz = 0.0f;

		m_model[index].x = top[numRSlice].x;
		m_model[index].y = top[numRSlice].y - Height * .5f;
		m_model[index].z = top[numRSlice].z;
		m_model[index].tu = top[numRSlice].tu;
		m_model[index].tv = top[numRSlice].tv;
		m_model[index].nx = 0.0f;
		m_model[index].ny = 1.0f;
		m_model[index++].nz = 0.0f;

		m_model[index].x = top[(i + 1)%numRSlice].x;
		m_model[index].y = top[(i + 1)%numRSlice].y - Height * .5f;
		m_model[index].z = top[(i + 1)%numRSlice].z;
		m_model[index].tu = top[(i + 1)%numRSlice].tu;
		m_model[index].tv = top[(i + 1)%numRSlice].tv;
		m_model[index].nx = 0.0f;
		m_model[index].ny = 1.0f;
		m_model[index++].nz = 0.0f;
		
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

bool ModelClass::CalculateBNT()
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

	return true;
}

void ModelClass::CalculateTangentBinormal(int i1, int i2, int i3)
{
	D3DXVECTOR3 P0, P1, P2;
	D3DXVECTOR3 Q1, Q2, B, T;
	float u11, u12, u21, u22;
	float size;

	P0 = D3DXVECTOR3(m_model[i1].x, m_model[i1].y, m_model[i1].z);
	P1 = D3DXVECTOR3(m_model[i2].x, m_model[i2].y, m_model[i2].z);
	P2 = D3DXVECTOR3(m_model[i3].x, m_model[i3].y, m_model[i3].z);

	Q1 = P1 - P0;
	Q2 = P2 - P0;

	u11 = m_model[i2].tu - m_model[i1].tu;
	u12 = m_model[i2].tv - m_model[i1].tv;//P1->texture.y - P0->texture.y;
	u21 = m_model[i3].tu - m_model[i1].tu;//P2->texture.x - P0->texture.x;
	u22 = m_model[i3].tv - m_model[i1].tv;//P2->texture.y - P0->texture.y;

	float detInv = u11*u22 - u21*u12;
	if(detInv == 0)
	{
		B = D3DXVECTOR3(0,0,0);
		T = D3DXVECTOR3(0,0,0);
	}
	else
	{
		//detInv = 1 / detInv;

		B = D3DXVECTOR3(u22*Q1 - u12*Q2);

		size = sqrt(B.x*B.x + B.y*B.y + B.z*B.z);
		B.x = B.x / size;
		B.y = B.y / size;
		B.z = B.z / size;


		T = D3DXVECTOR3(-u21*Q1 + u11*Q2);
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

	Q12 = P2 - P1;
	Q13 = P3 - P1;

	x = Q12.y*Q13.z - Q12.z*Q13.y;
	y = Q12.z*Q13.x - Q12.x*Q13.z;
	z = Q12.x*Q13.y - Q12.y*Q13.x;

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