// constant buffers

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrixl
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
	matrix 
};

// IO structs

struct VertexInput
{
	float3 position : POSITION;
	float2 tex      : TEXCOORD0;
	float3 normal   : NORMAL;
};

struct PixulInput
{
	float3 position : SV_POSITION;
	float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
};

// vertex shader

PixelInput BillboardVertexShader(VertexInput)
{
	float3 
}