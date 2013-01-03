cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

cbuffer TimeBuffer
{
	float time;
	float frequency;
	float phase;
	float time_padding;
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
};

float Scale(float t, float f, float p)
{
	return 1.0f+(2.0-cos(2*3.14159265*f*t - p))*.5f;
}

matrix RotateZ(float angle)
{
	matrix result = {cos(angle), -sin(angle), 0.0, 0.0,
					 sin(angle), cos(angle), 0.0, 0.0,
					 0.0, 0.0, 1.0, 0.0,
					 0.0, 0.0, 0.0, 1.0};
	return result;
}

PixelInput ParticleVertexShader(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

	output.position.xyz = 3 * Scale(time, frequency, phase) * input.position.xyz;
	output.position.w = 1.0f;
	output.position = mul(RotateZ(2*3.1415926*time*frequency / 10.0f - phase), output.position);

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	return output;
}