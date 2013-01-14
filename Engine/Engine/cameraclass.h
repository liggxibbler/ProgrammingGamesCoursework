////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void IncRotation(float, float, float);
	void IncPosition(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR3 GetRight();
	D3DXVECTOR3 GetUp();
	
	void GetBillboardAlign(D3DXMATRIX& out);
	void GetBillboardXZ(D3DXMATRIX& out, D3DXVECTOR3& pos);

	void Frame(int, int, bool, bool, bool, bool);
	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	//float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXVECTOR3 m_position;
	D3DXMATRIX m_viewMatrix;
	D3DXVECTOR3 m_direction;
	D3DXVECTOR3 m_right;
	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_velocity;
};

#endif