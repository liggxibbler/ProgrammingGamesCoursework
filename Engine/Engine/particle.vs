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
	float type;
	float3 time_padding;
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

	output.position = input.position;
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
	float v, y;
	
	v = .07;//a * time + frequency;
	y = v * time + phase*4;

	if(y>20)
	{
		y = y%20;
	}

	y = 20 - y;

	output.position = input.position;
	//////////////////////////////////////////////////
	// The problem below was that it was doing this :
	// VERY WRONG output.position.y = y
	// What it did was basically squash the whole polygon
	// to a line segment, with all three vertices at the same y
	// value. 
	output.position.y = output.position.y + y;
	output.position = mul(output.position, billboardMatrix);
	output.alpha = 1.0;
	
	return output;
}

PixelInput Fire(VertexInput input)
{
	PixelInput output;
	
	float y, t, s;
	
	t = (time * .001 + phase) % life;
	
	//y = frequency * t * 10;

	output.position = input.position;
	output.position.xyz *= t * 10;
	//output.position.y += y;
	output.position = mul(output.position, billboardMatrix);

	output.alpha = (life - t) * .2;

	return output;
}

PixelInput Spray(VertexInput input)
{
	PixelInput output;
	
	float x, y, z, t;
	
	t = (time * .001 + phase) % life;

	y = .5 * -9.8 * t * t + frequency * t;
	x = 2 * t * cos(frequency);
	z = 2 * t * sin(frequency);

	output.position = input.position;
	output.position.xyz *= t / life;
	
	output.position = mul(output.position, billboardMatrix);

	output.position.y += y;
	output.position.x += x;
	output.position.z += z;

	//output.alpha = (5 - t) * .2;
	output.alpha = (life - t) /life;

	return output;
}

/*
PixelInput WorldViewProjection(PixelInput input)
{
	PixelInput output;
	
	output.position = input.position;

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	return output;
}
//*/

PixelInput ParticleVertexShader(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

	if(type == 1)
	{
		output = Rain(input);
	}
	else if(type == 2)
	{
		output = RotoZoom(input);
	}
	else if(type == 3)
	{
		output = Spray(input);
	}
	else
	{
		output = Fire(input);
	}
	

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	return output;
}