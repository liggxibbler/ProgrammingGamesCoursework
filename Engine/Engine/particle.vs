cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix billboardMatrix;
}

cbuffer TimeBuffer
{
	float time;
	float frequency;
	float phase;
	float life;
}

cbuffer CameraBuffer
{
	float3 camPosition;
	float3 camAt;
	float3 camUp;
	float3 cam_padding;
}

struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float alpha : TEXCOORD1;
};

float Scale(float t, float f, float p)
{
	return 1.0f+(2.0-cos(2*3.14159265*f*t - p))*.5f;
}

matrix Billboard()
{
	float3 right;
	float3 at = normalize(camAt);
	float3 up = normalize(camUp);

	right = cross(at, up);
	right = normalize(right);

	matrix bb = {right.x, up.x, at.x, 0.0f,
				 right.y, up.y, at.y, 0.0f,
				 right.z, up.z, at.z, 0.0f,
				 0.0f   , 0.0f, 0.0f, 1.0f};
	
	//bb = transpose(bb);

	return bb;
}

matrix RotateZ(float angle)
{
	matrix result = {cos(angle), -sin(angle), 0.0, 0.0,
					 sin(angle), cos(angle), 0.0, 0.0,
					 0.0, 0.0, 1.0, 0.0,
					 0.0, 0.0, 0.0, 1.0};
	return result;
}

PixelInput RotoZoom(VertexInput input)
{
	PixelInput output;
	float sFactor;

	sFactor = Scale(time, frequency, phase);

	output.position.xyz = 3 * sFactor * input.position.xyz;
	output.position.w = 1.0f; // needed for matrix operations
	output.position = mul(RotateZ(2*3.1415926*time*frequency / 10.0f - phase), output.position);

	output.alpha = 1.0 - (sFactor - 1.5);
	
	output.position = mul(output.position, billboardMatrix);
	//output.position = mul(output.position, Billboard());

	output.tex = input.tex;
	return output;
}

PixelInput Rain(VertexInput input)
{
	PixelInput output;
	
	output.position = input.position;
	
	return output;
}


PixelInput ParticleVertexShader(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

	output = RotoZoom(input);
	//output = Rain(input);

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	return output;
}