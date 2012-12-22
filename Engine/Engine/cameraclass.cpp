////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_velocity = D3DXVECTOR3(0,0,0);
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	//m_positionX = x;
	//m_positionY = y;
	//m_positionZ = z;
	m_position = D3DXVECTOR3(x,y,z);
	return;
}

void CameraClass::IncPosition(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void CameraClass::IncRotation(float x, float y, float z)
{
	m_rotationX += x;
	m_rotationY += y;
	m_rotationZ += z;
	return;
}


D3DXVECTOR3 CameraClass::GetPosition()
{
	//return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
	return m_position;
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

D3DXVECTOR3 CameraClass::GetDirection()
{
	float mag = m_direction.x * m_direction.x + m_direction.y * m_direction.y + m_direction.z * m_direction.z;
	mag = sqrt(mag);
	m_direction /= mag;
	return m_direction;
}

void CameraClass::Render()
{
	D3DXVECTOR3 lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	m_up.x = 0.0f;
	m_up.y = 1.0f;
	m_up.z = 0.0f;

	// Setup the position of the camera in the world.
	//position.x = m_positionX;
	//position.y = m_positionY;
	//position.z = m_positionZ;

	// Setup where the camera is looking by default.
	m_direction.x = 0.0f;
	m_direction.y = 0.0f;
	m_direction.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&m_direction, &m_direction, &rotationMatrix);
	D3DXVec3TransformCoord(&m_up, &m_up, &rotationMatrix);

	//m_direction = D3DXVECTOR3(lookAt);

	m_right.x = -(m_direction.y*m_up.z - m_direction.z*m_up.y);
	m_right.y = -(m_direction.z*m_up.x - m_direction.x*m_up.z);
	m_right.z = -(m_direction.x*m_up.y - m_direction.y*m_up.x);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = m_position + m_direction;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &lookAt, &m_up);

	return;
}


void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

D3DXVECTOR3 CameraClass::GetRight()
{
	return m_right;
}

D3DXVECTOR3 CameraClass::GetUp()
{
	return m_up;
}

void CameraClass::Frame(int xMouse, int yMouse, bool w, bool a, bool s, bool d)
{
	float step = .5, mag;
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);

	float x = static_cast<float>(xMouse);
	float y = static_cast<float>(yMouse);

	IncRotation(y / 5.0f, x / 5.0f, 0);
	
	if (w)
	{
		at += m_direction;
	}
	if (s)
	{
		at -= m_direction;
	}
	if (a)
	{
		at -= m_right;
	}
	if (d)
	{
		at += m_right;
	}

	//m_positionX += step * at.x;
	//m_positionY += step * at.y;
	//m_positionZ += step * at.z;
	m_position += step * at;
}