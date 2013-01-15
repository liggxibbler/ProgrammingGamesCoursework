Texture2D heightmap;
SamplerState smapler;

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
	 float3 normal : NORMAL;
	 float alpha1 : TEXCOORD1;
	 float alpha2 : TEXCOORD2;
};

PixelInput HeightmapVertexShader(VertexInput input)
{
	float4 map;
	float heightTex, tex1, tex2;
	float scale = 10;

	//float diff = 0.001953125f; // = (1 / 512)

	map = heightmap.Sample(sampler, input.tex);

	output.position.y = map.r;
	output.position.x = input.tex.x;
	output.position.z = -input.tex.v;

	output.position.xyz *= scale;
	output.w = 1.0f;

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.alpha1 = map.g;
	output.alpha2 = map.b;
}

/*
	map[0] = heightmap.Sample(sampler, input.tex);
	map[1] = heightmap.Sample(sampler, input.tex + float2(diff,-diff));
	map[2] = heightmap.Sample(sampler, input.tex + float2(diff,0));
	map[3] = heightmap.Sample(sampler, input.tex + float2(diff,diff));
	map[4] = heightmap.Sample(sampler, input.tex + float2(0,diff));
	map[5] = heightmap.Sample(sampler, input.tex + float2(-diff,diff));
	map[6] = heightmap.Sample(sampler, input.tex + float2(-diff,0));
	map[7] = heightmap.Sample(sampler, input.tex + float2(-diff,-diff));
	map[8] = heightmap.Sample(sampler, input.tex + float2(0,-diff));

	gradient[0] = (map[0].r - map[1].r) / diff * cos(0.78539);
	gradient[1] = (map[0].r - map[2].r) / diff;
	gradient[2] = (map[0].r - map[3].r) / diff * cos(0.78539);
	gradient[3] = (map[0].r - map[4].r) / diff;
	gradient[4] = (map[0].r - map[5].r) / diff * cos(0.78539);
	gradient[5] = (map[0].r - map[6].r) / diff;
	gradient[6] = (map[0].r - map[7].r) / diff * cos(0.78539);
	gradient[7] = (map[0].r - map[8].r) / diff;
	//*/