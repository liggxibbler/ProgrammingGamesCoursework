////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void CameraClass::IncPosition(float x, float y, float z)
{
	m_positionX += x;
	m_positionY += y;
	m_positionZ += z;
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
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
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
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	m_direction = D3DXVECTOR3(lookAt);

	m_right.x = -(lookAt.y*up.z - lookAt.z*up.y);
	m_right.y = -(lookAt.z*up.x - lookAt.x*up.z);
	m_right.z = -(lookAt.x*up.y - lookAt.y*up.x);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

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

void CameraClass::Frame(int xMouse, int yMouse, bool w, bool a, bool s, bool d)
{
	float step = .1;
	D3DXVECTOR3 at;

	float x = (float)(100 * xMouse) / 100.f; //  not sure why this works! but it adds to the smoothness
	float y = (float)(100 * yMouse) / 100.f; // of the mouse camera control. //XIBB//

	IncRotation(y / 5.0f, x / 5.0f, 0);
	
	if (w)
	{
		at = m_direction * step;
		IncPosition(at.x, at.y, at.z);
	}
	if (s)
	{
		at = m_direction * -step;
		IncPosition(at.x, at.y, at.z);
	}
	if (a)
	{
		at = m_right * -step;
		IncPosition(at.x, at.y, at.z);
	}
	if (d)
	{
		at = m_right * step;
		IncPosition(at.x, at.y, at.z);
	}
}