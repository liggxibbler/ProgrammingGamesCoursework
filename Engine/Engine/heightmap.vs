Texture2D heightmap;
SamplerState vsSampler;

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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
	 float alpha1 : TEXCOORD1;
	 float alpha2 : TEXCOORD2;
};

PixelInput HeightmapVertexShader(VertexInput input)
{
	PixelInput output;
	float4 map;
	float scale = 100;

	map = heightmap.SampleLevel(vsSampler, input.tex, 0);

	output.position.y = map.r * 1;
	output.position.x = input.tex.x - .5;
	output.position.z = .5 - input.tex.y;
	
	output.position.w = 1.0f;

	//output.position = input.position;
	//output.position.y = map.r * 10;

	output.position.xz *= scale;
	output.position.y *= scale * .1;

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex * scale;
	output.alpha1 = map.g;
	output.alpha2 = map.b;

	//output.position.xy = input.tex * .5 - (float2).1;
	//output.position.z = map.r - .5;

	return output;
}