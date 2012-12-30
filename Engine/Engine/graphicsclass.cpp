////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include <time.h>

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Model2 = 0;
	m_Sphere = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_positions = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), 200, 0, 4, 1, 100, L"../data/grass.png", L"../data/BTSn.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize ground mesh object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model2 = new ModelClass;
	if(!m_Model2)
	{
		return false;
	}

	// Initialize the model object.
	//result = m_Model2->Initialize(m_D3D->GetDevice(), 10, 25, 20, 1, 2, L"../data/standing.png");
	result = m_Model2->Initialize(m_D3D->GetDevice(), "../data/bill.txt", L"../data/tree.dds", L"../data/SnakeScale.jpg");
	//result = m_Model2->Initialize(m_D3D->GetDevice(), 3, 5, 20, 1, 1, L"../data/BPT.png", L"../data/BPTn.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize billboard object.", L"Error", MB_OK);
		return false;
	}

	m_Model2->CalculateBNT(m_D3D->GetDevice());

	// Create the model object.
	m_Sphere = new ModelClass;
	if(!m_Sphere)
	{
		return false;
	}

	// Initialize the model object.
	//result = m_Model2->Initialize(m_D3D->GetDevice(), 10, 25, 20, 1, 2, L"../data/standing.png");
	result = m_Sphere->Initialize(m_D3D->GetDevice(), "../data/sphere.txt", L"../data/mountain.png", L"../data/BTSn.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize skydome object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	float x, y, z;
	m_positions = new D3DXVECTOR3[100];
	if(!m_positions)
	{
		return false;
	}
	srand(time(NULL));
	for(int i =0; i<100; i++)
	{
		x = ((rand() % 100) / 100.0f) * 40 - 20.0;
		y = 0;
		z = ((rand() % 100) / 100.0f) * 40 - 20.0;
		m_positions[i].x = x;
		m_positions[i].y = y;
		m_positions[i].z = z;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(100.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	if(m_positions)
	{
		delete [] m_positions;
		m_positions = 0;
	}
	
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(GraphicsClass::GraphicsUpdateInfo& guInf)
{
	bool result;
	static float rotation = 0.0f;
	
	m_Camera->Frame(guInf.mouseDiffX, guInf.mouseDiffY, guInf.wKey, guInf.aKey, guInf.sKey, guInf.dKey);

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXMATRIX cameraRot, temp;
	D3DXVECTOR3 up, right, at, cameraPos;

	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	cameraPos = m_Camera->GetPosition();

	m_D3D->TurnZBufferOff();
	
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixTranslation(&worldMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
	//temp = D3DXMATRIX(1,0,0,0, 0,1,0,0, 0,0,-1,0, 0,0,0,1); // turn the sphere inside out, very bad idea
	m_D3D->CullFrontFace();


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Sphere->Render(m_D3D->GetDeviceContext(), worldMatrix);
	//D3DXMatrixMultiply(&worldMatrix, &temp, &worldMatrix);

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Sphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Sphere->GetTexture(), m_Light->GetDirection(), D3DXVECTOR4(1,1,1,1), D3DXVECTOR4(0,0,0,1), 
								   m_Camera->GetPosition(),  D3DXVECTOR4(0,0,0,1), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}

	m_D3D->TurnZBufferOn();
	m_D3D->CullBackFace();


	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixTranslation(&worldMatrix, 0, -1, 0);
	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext(), worldMatrix);

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								   m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
								   m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}

	srand(time(NULL)*1000.0f);

	struct zSort
	{
		float z;
		int i;
		zSort(){}
		zSort(float _z, int _i)
		{
			z = _z;
			i = _i;
		}
	};

	zSort zPos[100], zTemp;
	float zi, zj;
	D3DXVECTOR3 tempVec;
	m_Camera->GetViewMatrix(temp);
	
	for(int i =0; i<100; i++)
	{
		D3DXVec3TransformCoord(&tempVec, &m_positions[i], &temp);
		zPos[i].z = tempVec.z;
		zPos[i].i = i;
	}
	// sort positions by z value, using the aweful bubble sort algorithm
	for(int i=0;i<100;i++)
	{
		for(int j = i; j<100; j++)
		{
			zi = zPos[i].z;
			zj = zPos[j].z;
			if(zi > zj)
			{
				zTemp = zSort(zPos[i].z, zPos[i].i);
				zPos[i] = zSort(zPos[j].z, zPos[j].i);
				zPos[j] = zSort(zTemp.z, zTemp.i);
			}
		}
	}


	m_D3D->TurnAlphaBlendingOn();
	for(int i = 0; i< 100; i++)
	{
		//int j = 99 - zPos[i].i;
		int j = i;

		m_Camera->GetBillboardAlign(cameraRot, m_positions[j]);
		//m_D3D->GetWorldMatrix(cameraRot);

		m_D3D->GetWorldMatrix(worldMatrix);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &cameraRot);
		D3DXMatrixTranslation(&temp, m_positions[j].x, 0, m_positions[j].z);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &temp);
		

		m_Model2->Render(m_D3D->GetDeviceContext(), worldMatrix);
		
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
			m_Model2->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if(!result)
		{
			return false;
		}

	}
	m_D3D->TurnAlphaBlendingOff();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}