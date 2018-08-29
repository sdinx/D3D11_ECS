//-----------------------------------------------------------------------------------
// CBPerFrame constant buffer
//-----------------------------------------------------------------------------------
cbuffer  CBuffer : register( b0 )
{
		float4x4  view;
		float4x4  proj;
		float4  light;
};

cbuffer  CObject : register( b1 )
{
		float4x4  world;
		float4  meshColor;
};
/*
cbuffer  CDirectionLight : register ( b2 )
{
		float4  dir;
		float4  color;
};

cbuffer  CPointLight : register ( b3 )
{
		float3  pos;
		float4  color;
};*/

//-----------------------------------------------------------------------------------
// Texture variables
//-----------------------------------------------------------------------------------
Texture2D  diffuseTexture : register( t0 );
SamplerState  diffuseTextureSampler : register( s0 );

//-----------------------------------------------------------------------------------
// VSInput structure
//-----------------------------------------------------------------------------------
struct VSInput
{
		float4  position				: POSITION;
		float3  normal					: NORMAL;
		float2  texcoord				: TEXCOORD0;
};

//-----------------------------------------------------------------------------------
// GSPSInput structure
//-----------------------------------------------------------------------------------
struct GSPSInput
{
		float4  position				: SV_POSITION;
		float3  normal					: NORMAL;
		float2  texcoord				: TEXCOORD0;
};


const float PI = 3.1415926535f;