cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

Texture2D heightmap;
SamplerState vsSampler;

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
	float scale = 200;

	map = heightmap[input.tex];

	output.position = input.position;
	//output.position.y = map.r * 5;
	//output.position.x = input.tex.x;
	//output.position.z = -input.tex.y;

	output.position.xz *= scale;
	output.position.w = 1.0f;

	output.position = mul(output.position, worldMatrix);
	//output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex * scale;
	output.alpha1 = 1;//map.g;
	output.alpha2 = 1;//map.b;

	return output;
}