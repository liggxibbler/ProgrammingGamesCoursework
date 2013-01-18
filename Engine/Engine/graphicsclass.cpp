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
	m_Ground2 = 0;

	m_bbClouds = 0;
	m_bbRain = 0;
	m_bbSnow = 0;
	m_bbTrees = 0;

	m_fire = 0;
	m_clouds = 0;
	m_rain = 0;
	m_snow = 0;
	m_trees = 0;

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
	float params[6];
	float rParams[4];

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

	m_Camera->SetPosition(0.0f, 20.0f, -40.0f);
	m_Camera->SetRotation(-D3DX_PI/6, 0.0f, 0.0f);
	m_Camera->Render();

	// Create the ground mesh object.
	m_Groundmesh = new ModelClass;
	if(!m_Groundmesh)
	{
		return false;
	}

	// Initialize the ground mesh object.
	result = m_Groundmesh->Initialize(m_D3D->GetDevice(), 64, 64, 1, 1, L"../data/hmaprgb.png", L"../data/rock.png", L"../data/grass.png", L"../data/dirt.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize ground mesh object.", L"Error", MB_OK);
		return false;
	}

	// Create the second ground mesh object.
	m_Ground2 = new ModelClass;
	if(!m_Ground2)
	{
		return false;
	}

	// Initialize the second ground mesh object.
	result = m_Ground2->Initialize(m_D3D->GetDevice(), 64, 64, 1, 1, L"../data/grass.png", L"../data/floor2_ddn.jpg", NULL, NULL);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize ground second mesh object.", L"Error", MB_OK);
		return false;
	}

	/////////////////////////////
	/// fire
	// Create the fire model object.
	m_bbTrees = new ModelClass;
	if(!m_Billboard)
	{

		return false;
	}

	// Initialize the fire model object.
	result = m_Billboard->Initialize(m_D3D->GetDevice(), "../data/bill.txt", L"../data/tree.png", L"../data/SnakeScale.jpg");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize fire billboard object.", L"Error", MB_OK);
		return false;
	}

	m_trees = new ParticleClass;
	if(!m_trees)
	{
		return false;
	}

	params[0] = -10; params[1] = 10; params[2] = 0; params[3] = 0; params[4] = -10; params[5] = 10;
	rParams[0] = 0; rParams[1] = 5; rParams[2] = 0; rParams[3] = .1;
	m_trees = new ParticleClass;
	m_trees->Initialize(100, params, rParams, 3.0, true, 0);

	/////////////////////////////


	/////////////////////////////
	/// fire
	// Create the fire model object.
	m_Billboard = new ModelClass;
	if(!m_Billboard)
	{

		return false;
	}

	// Initialize the fire model object.
	result = m_Billboard->Initialize(m_D3D->GetDevice(), "../data/bill.txt", L"../data/fire.png", L"../data/SnakeScale.jpg");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize fire billboard object.", L"Error", MB_OK);
		return false;
	}

	m_fire = new ParticleClass;
	if(!m_fire)
	{
		return false;
	}

	params[0] = -10; params[1] = 10; params[2] = 0; params[3] = 0; params[4] = -10; params[5] = 10;
	rParams[0] = 0; rParams[1] = 5; rParams[2] = 0; rParams[3] = .1;
	m_fire = new ParticleClass;
	m_fire->Initialize(100, params, rParams, 3.0, true, 0);

	/////////////////////////////

	/////////////////////////////
	/// clouds
	// Create the clouds model object.
	m_bbClouds = new ModelClass;
	if(!m_bbClouds)
	{

		return false;
	}

	// Initialize the fire model object.
	result = m_bbClouds->Initialize(m_D3D->GetDevice(), "../data/bill.txt", L"../data/cloud3.png", NULL);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize cloud billboard object.", L"Error", MB_OK);
		return false;
	}

	m_clouds = new ParticleClass;
	if(!m_clouds)
	{
		return false;
	}

	params[0] = -10; params[1] = 10; params[2] = 20; params[3] = 20; params[4] = -10; params[5] = 10;
	rParams[0] = 0; rParams[1] = 5; rParams[2] = .0005; rParams[3] = .0005;
	m_clouds = new ParticleClass;
	m_clouds->Initialize(200, params, rParams, 3.0, true, 2);

	/////////////////////////////

	/////////////////////////////
	/// rain
	// Create the rain model object.
	m_bbRain = new ModelClass;
	if(!m_bbRain)
	{

		return false;
	}

	// Initialize the fire model object.
	result = m_bbRain->Initialize(m_D3D->GetDevice(), "../data/bill.txt", L"../data/rain.png", NULL);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize rain billboard object.", L"Error", MB_OK);
		return false;
	}

	params[0] = -10; params[1] = 10; params[2] = 0; params[3] = 0; params[4] = -10; params[5] = 10;
	rParams[0] = 0; rParams[1] = 3; rParams[2] = 0; rParams[3] = .001;
	m_rain = new ParticleClass;
	m_rain->Initialize(100, params, rParams, 3.0, true, 1);
	/////////////////////////////

	/////////////////////////////
	/// snow
	// Create the clouds model object.
	m_bbSnow = new ModelClass;
	if(!m_bbSnow)
	{

		return false;
	}

	// Initialize the snow model object.
	result = m_bbSnow->Initialize(m_D3D->GetDevice(), "../data/bill.txt", L"../data/snow.png", NULL);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize snow billboard object.", L"Error", MB_OK);
		return false;
	}

	params[0] = -10; params[1] = 10; params[2] = 0; params[3] = 0; params[4] = -10; params[5] = 10;
	rParams[0] = 0; rParams[1] = 2*D3DX_PI; rParams[2] = 0; rParams[3] = .1;
	m_snow = new ParticleClass;
	m_snow->Initialize(100, params, rParams, 3.0, true, 3);

	/////////////////////////////

	// Create the skydome model object.
	m_Sphere = new ModelClass;
	if(!m_Sphere)
	{
		return false;
	}

	// Initialize the skydome model object.
	result = m_Sphere->Initialize(m_D3D->GetDevice(), "../data/sphere.txt", L"../data/day2.png", L"../data/night2.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize skydome object.", L"Error", MB_OK);
		return false;
	}

	m_Sphere->ScaleTexCoords(0.5f, 1.0f, m_D3D->GetDevice()); // adjust the texture coordinates so it doesn't wrap twice

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

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(0.4f, 0.4f, 0.4f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(10.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	if(m_trees)
	{
		m_trees->Shutdown();
		delete m_trees;
		m_trees = 0;
	}

	if(m_bbTrees)
	{
		m_bbTrees->Shutdown();
		delete m_bbTrees;
		m_bbTrees = 0;
	}

	if(m_fire)
	{
		m_fire->Shutdown();
		delete m_fire;
		m_fire = 0;
	}

	if(m_bbClouds)
	{
		m_bbClouds->Shutdown();
		delete m_bbClouds;
		m_bbClouds = 0;
	}

	if(m_bbRain)
	{
		m_bbRain->Shutdown();
		delete m_bbRain;
		m_bbRain = 0;
	}

	if(m_bbSnow)
	{
		m_bbSnow->Shutdown();
		delete m_bbSnow;
		m_bbSnow = 0;
	}

	if(m_clouds)
	{
		m_clouds->Shutdown();
		delete m_clouds;
		m_clouds = 0;
	}

	if(m_rain)
	{
		m_rain->Shutdown();
		delete m_rain;
		m_rain = 0;
	}

	if(m_snow)
	{
		m_snow->Shutdown();
		delete m_snow;
		m_snow = 0;
	}

	if(m_Ground2)
	{
		m_Ground2->Shutdown();
		delete m_Ground2;
		m_Ground2 = 0;
	}

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

	////////////////////////////////////////////////
	/// Skydome

	m_D3D->TurnZBufferOff();
	
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// move the center of the sphere to the camera position
	D3DXMatrixTranslation(&worldMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
	m_D3D->CullFrontFace(); // so the inside of the sphere is visible


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Sphere->Render(m_D3D->GetDeviceContext(), worldMatrix);
	
	//*// Render the model using the alpha shader.
	result = m_alphaFadeShader->Render(m_D3D->GetDeviceContext(), m_Sphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Sphere->GetTexture(), (1 + cos(time * .001f)) * .5);
	
	if(!result)
	{
		return false;
	}
	//*/
	m_D3D->TurnZBufferOn();
	m_D3D->CullBackFace();
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	// ground mesh

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Groundmesh->Render(m_D3D->GetDeviceContext(), worldMatrix);

	/*D3DXMatrixScaling(&worldMatrix, 100, 100, 100);
	D3DXMatrixTranslation(&tempMatrix, 300, 0.0, 0.0);
	D3DXMatrixMultiply(&worldMatrix, &tempMatrix, &worldMatrix);*/

	//*/ Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Groundmesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								   m_Groundmesh->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
								   m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	//*/

	m_D3D->GetWorldMatrix(worldMatrix);

	//*/ Render using terrain shader
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Groundmesh->Render(m_D3D->GetDeviceContext(), worldMatrix);
	result = m_terrainShader->Render(m_D3D->GetDeviceContext(), m_Groundmesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								   m_Groundmesh->GetTexture());
	//*/

	////////////////////////////////////////////////

	////////////////////////////////////////////////
	/// test shader example

	/*/
	m_testShader->ClearStacks();
	m_testShader->PushMatrix(&projectionMatrix);
	m_testShader->PushMatrix(&viewMatrix);
	m_testShader->PushMatrix(&worldMatrix);
	m_testShader->PushResourceView(m_Groundmesh->GetTexture()[0]);
	result = m_testShader->Render(m_D3D->GetDeviceContext(), m_Groundmesh->GetIndexCount());
	//*/

	////////////////////////////////////////////////

	if(!result)
	{
		return false;
	}
	
	////////////////////////////////////////////////
	/// render particles

	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->TurnAlphaBlendingOn();
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);
	
	int j;
	m_clouds->SortByZ(viewMatrix);
	for(int i = 0; i< m_clouds->GetCount(); i++)
	{
		j = m_clouds->GetIndex(i); // this line is very important: get the next index in the z-sorted array
		m_Camera->GetBillboardAlign(cameraRot);
		D3DXMatrixTranslation(&worldMatrix, m_clouds->GetPositions()[j].x, m_clouds->GetPositions()[j].y, m_clouds->GetPositions()[j].z);	

		m_bbClouds->Render(m_D3D->GetDeviceContext(), worldMatrix);
		
		result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_bbClouds->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
			cameraRot, m_Camera->GetPosition(), m_Camera->GetDirection(), m_Camera->GetUp(), m_bbClouds->GetTexture(),
			time, m_clouds->GetSpeeds()[j], m_clouds->GetPhases()[j], m_clouds->GetLife(), m_clouds->GetType());
		if(!result)
		{
			return false;
		}

	}

	m_rain->SortByZ(viewMatrix);
	for(int i = 0; i< m_rain->GetCount(); i++)
	{
		j = m_rain->GetIndex(i); // this line is very important: get the next index in the z-sorted array
		m_Camera->GetBillboardAlign(cameraRot);
		D3DXMatrixTranslation(&worldMatrix, m_rain->GetPositions()[j].x, m_rain->GetPositions()[j].y, m_rain->GetPositions()[j].z);	

		m_bbRain->Render(m_D3D->GetDeviceContext(), worldMatrix);
		
		result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_bbRain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
			cameraRot, m_Camera->GetPosition(), m_Camera->GetDirection(), m_Camera->GetUp(), m_bbRain->GetTexture(),
			time, m_rain->GetSpeeds()[j], m_rain->GetPhases()[j], m_rain->GetLife(), m_rain->GetType());
		if(!result)
		{
			return false;
		}

	}

	m_fire->SortByZ(viewMatrix);
	for(int i = 0; i< m_fire->GetCount(); i++)
	{
		j = m_fire->GetIndex(i); // this line is very important: get the next index in the z-sorted array
		m_Camera->GetBillboardAlign(cameraRot);
		D3DXMatrixTranslation(&worldMatrix, m_fire->GetPositions()[j].x, m_fire->GetPositions()[j].y, m_fire->GetPositions()[j].z);	

		m_Billboard->Render(m_D3D->GetDeviceContext(), worldMatrix);
		
		result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_Billboard->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
			cameraRot, m_Camera->GetPosition(), m_Camera->GetDirection(), m_Camera->GetUp(), m_Billboard->GetTexture(),
			time, m_fire->GetSpeeds()[j], m_fire->GetPhases()[j], m_fire->GetLife(), m_fire->GetType());
		if(!result)
		{
			return false;
		}

	}

	////////////////////////////////////////////////

	////////////////////////////////////////////////
	/// text display

	static D3DXMATRIX orthoMatrix;
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);

	m_D3D->TurnAlphaBlendingOff();
	m_D3D->TurnZBufferOn();
	////////////////////////////////////////////////

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}