////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include <time.h>

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Groundmesh = 0;
	m_Billboard = 0;
	m_Sphere = 0;
	m_Text = 0;

	m_LightShader = 0;
	m_alphaFadeShader = 0;
	m_BitmapShader = 0;
	m_ParticleShader = 0;
	m_testShader = 0;
	m_terrainShader = 0;
	m_FontShader = 0;
	
	m_Light = 0;
	m_positions = 0;
	m_phases = 0;
	m_speeds = 0;
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
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();

	m_Text = new TextClass;
	if(!m_Text)
	{
		 return false;
	}

	D3DXMATRIX matrix;
	m_Camera->GetViewMatrix(matrix);
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, matrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize text object.", L"Error", MB_OK);
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_Groundmesh = new ModelClass;
	if(!m_Groundmesh)
	{
		return false;
	}

	// Initialize the model object.
	//result = m_Groundmesh->Initialize(m_D3D->GetDevice(), 50, 0, 20, 20, 20, L"../data/rock.png", L"../data/floor2_ddn.jpg");
	//result = m_Groundmesh->Initialize(m_D3D->GetDevice(), 200, 0, 4, 1, 500, L"../data/grass.png", L"../data/sphere.png");
	//result = m_Groundmesh->Initialize(m_D3D->GetDevice(), 64, 64, 1, 1, L"../data/hmaprgb.png", L"../data/dirt.png", L"../data/grass.png", L"../data/rock.png");
	result = m_Groundmesh->Initialize(m_D3D->GetDevice(), 64, 64, 1, 1, L"../data/hmaprgb.png", L"../data/floor2_ddn.jpg", L"../data/grass.png", L"../data/rock.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize ground mesh object.", L"Error", MB_OK);
		return false;
	}

	m_Groundmesh->CalculateBNT(m_D3D->GetDevice());

	// Create the model object.
	m_Billboard = new ModelClass;
	if(!m_Billboard)
	{

		return false;
	}

	// Initialize the model object.
	//result = m_Billboard->Initialize(m_D3D->GetDevice(), 10, 25, 20, 1, 2, L"../data/standing.png");
	result = m_Billboard->Initialize(m_D3D->GetDevice(), "../data/bill.txt", L"../data/fire.png", L"../data/SnakeScale.jpg");
	//result = m_Billboard->Initialize(m_D3D->GetDevice(), .3, .7, 20, 1, 1, L"../data/BTS.png", L"../data/BTSn.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize billboard object.", L"Error", MB_OK);
		return false;
	}

	m_Billboard->CalculateBNT(m_D3D->GetDevice());

	// Create the model object.
	m_Sphere = new ModelClass;
	if(!m_Sphere)
	{
		return false;
	}

	// Initialize the model object.
	//result = m_Billboard->Initialize(m_D3D->GetDevice(), 10, 25, 20, 1, 2, L"../data/standing.png");
	result = m_Sphere->Initialize(m_D3D->GetDevice(), "../data/sphere.txt", L"../data/day2.png", L"../data/night2.png");
	//result = m_Sphere->Initialize(m_D3D->GetDevice(), "../data/sphere.txt", L"../data/mountain_day.png", L"../data/BTSn.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize skydome object.", L"Error", MB_OK);
		return false;
	}

	// m_Sphere->SetSphericalTexCoords(m_D3D->GetDevice()); // doesn't work well. described in method body
	m_Sphere->ScaleTexCoords(0.5f, 1.0f, m_D3D->GetDevice());

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

	// Create the bitmap shader object.
	m_BitmapShader = new BitmapShaderClass;
	if(!m_BitmapShader)
	{
		return false;
	}

	// Initialize the bitmap shader object.
	result = m_BitmapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the test shader object.
	m_testShader = new TestGenericShader;
	if(!m_testShader)
	{
		return false;
	}

	// Initialize the test shader object.
	result = m_testShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the test shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderClass;
	if(!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the alpha shader object.
	m_alphaFadeShader = new AlphaFadeShaderClass;
	if(!m_alphaFadeShader)
	{
		return false;
	}

	// Initialize the alpha shader object.
	result = m_alphaFadeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the aplha shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_terrainShader = new TerrainShaderClass;
	if(!m_terrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_terrainShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_FontShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
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
	m_phases = new float[100];
	if(!m_phases)
	{
		return false;
	}
	m_speeds = new float[100];
	if(!m_speeds)
	{
		return false;
	}
	srand(time(NULL));
	for(int i =0; i<100; i++)
	{
		x = floorf(i / 10)/10.0 * 40.0 - 20.0;
		//x = (rand() % 10) / 10.0 * 40 - 20.0;
		y = 0;
		z = (i % 10)/10.0 * 40.0 - 20.0;
		//z = (rand() % 10) / 10.0 * 40 - 20.0;
		m_positions[i].x = x;
		m_positions[i].y = y;
		m_positions[i].z = z;
		m_phases[i] = (rand() % 100)/100.0 * 2 * D3DX_PI;
		m_speeds[i] = (rand() % 100)/100.0 * 2 * D3DX_PI;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetDiffuseColor(0.4f, 0.4f, 0.4f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(10.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	if(m_positions)
	{
		delete [] m_positions;
		m_positions = 0;
	}
	
	if(m_phases)
	{
		delete [] m_phases;
		m_phases = 0;
	}

	if(m_speeds)
	{
		delete [] m_speeds;
		m_speeds = 0;
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

	if(m_terrainShader)
	{
		m_terrainShader->Shutdown();
		delete m_terrainShader;
		m_terrainShader = 0;
	}

	if(m_testShader)
	{
		m_testShader->Shutdown();
		delete m_testShader;
		m_testShader = 0;
	}

	if(m_BitmapShader)
	{
		m_BitmapShader->Shutdown();
		delete m_BitmapShader;
		m_BitmapShader = 0;
	}

	if(m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	if(m_alphaFadeShader)
	{
		m_alphaFadeShader->Shutdown();
		delete m_alphaFadeShader;
		m_alphaFadeShader = 0;
	}

	// Release the model object.
	if(m_Groundmesh)
	{
		m_Groundmesh->Shutdown();
		delete m_Groundmesh;
		m_Groundmesh = 0;
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
	static float time = 0.0f;
	
	static D3DXMATRIX tempMat;
	static D3DXVECTOR3 tempVec;

	time += guInf.time;

	m_Camera->Frame(guInf.mouseDiffX, guInf.mouseDiffY, guInf.wKey, guInf.aKey, guInf.sKey, guInf.dKey);

	tempVec = D3DXVECTOR3(m_Light->GetDirection());
	D3DXMatrixRotationZ(&tempMat, guInf.time * .001f);
	D3DXVec3TransformCoord(&tempVec, &tempVec, &tempMat);
	m_Light->SetDirection(tempVec.x, tempVec.y, tempVec.z);
	
	m_Text->SetCpu(guInf.cpu, m_D3D->GetDeviceContext());
	m_Text->SetFps(guInf.fps, m_D3D->GetDeviceContext());

	// Render the graphics scene.
	result = Render(time);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float time)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXMATRIX cameraRot, tempMatrix;
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

	//*// Render the model using the alpha shader.
	result = m_alphaFadeShader->Render(m_D3D->GetDeviceContext(), m_Sphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Sphere->GetTexture(), (1 + cos(time * .001f)) * .5);
	
	if(!result)
	{
		return false;
	}
	//*/

	/*// Render the skydome using the light shader.
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Sphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
	//	m_Sphere->GetTexture(), D3DXVECTOR3(0,0,0), D3DXVECTOR4(1.0, 1.0, 1.0f, 1.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),  
	//	D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), 0.0f);
	//if(!result)
	//{
	//	return false;
	//}
	//*/

	m_D3D->TurnZBufferOn();
	m_D3D->CullBackFace();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//D3DXMatrixScaling(&worldMatrix, 200, 200, 200);
	//D3DXMatrixRotationX(&worldMatrix, 90);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Groundmesh->Render(m_D3D->GetDeviceContext(), worldMatrix);

	//*/ Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Groundmesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								   m_Groundmesh->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
								   m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	//*/

	//*/ Render using terrain shader
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Groundmesh->Render(m_D3D->GetDeviceContext(), worldMatrix);
	result = m_terrainShader->Render(m_D3D->GetDeviceContext(), m_Groundmesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								   m_Groundmesh->GetTexture());
	//*/

	/*/
	m_testShader->ClearStacks();
	m_testShader->PushMatrix(&projectionMatrix);
	m_testShader->PushMatrix(&viewMatrix);
	m_testShader->PushMatrix(&worldMatrix);
	m_testShader->PushResourceView(m_Groundmesh->GetTexture()[0]);
	result = m_testShader->Render(m_D3D->GetDeviceContext(), m_Groundmesh->GetIndexCount());
	//*/

	if(!result)
	{
		return false;
	}
	

	m_D3D->GetProjectionMatrix(projectionMatrix);

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
	m_Camera->GetViewMatrix(tempMatrix);
	
	for(int i =0; i<100; i++)
	{
		D3DXVec3TransformCoord(&tempVec, &m_positions[i], &tempMatrix);
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
	//m_D3D->TurnZBufferOff();
	for(int i = 0; i< 100; i++)
	{
		int j = 99 - zPos[i].i;
		//int j = i;

		m_Camera->GetBillboardAlign(cameraRot);
		//m_D3D->GetWorldMatrix(cameraRot);

		m_D3D->GetWorldMatrix(worldMatrix);
		//D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &cameraRot);
		D3DXMatrixTranslation(&tempMatrix, m_positions[j].x, 0, m_positions[j].z);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &tempMatrix);
		

		m_Billboard->Render(m_D3D->GetDeviceContext(), worldMatrix);
		
		result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_Billboard->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
			cameraRot, m_Camera->GetPosition(), m_Camera->GetDirection(), m_Camera->GetUp(), m_Billboard->GetTexture(), time, m_speeds[j], m_phases[j], 3.0f, 0);
		if(!result)
		{
			return false;
		}

	}

	static D3DXMATRIX orthoMatrix;
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);

	m_D3D->TurnAlphaBlendingOff();
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}