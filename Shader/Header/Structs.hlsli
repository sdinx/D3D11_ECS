//-----------------------------------------------------------------------------------
// CBPerFrame constant buffer
//-----------------------------------------------------------------------------------
cbuffer  CBuffer : register( b0 )
{
		float4x4  g_view;
		float4x4  g_proj;
		float4x4  g_invView;
};

cbuffer  CObject : register( b1 )
{
		float4x4  g_world;
		float4  g_ambient;
		float4  g_diffuse;
		float4  g_emissive;
		float4  g_specular;
};


//-----------------------------------------------------------------------------------
// Texture variables
//-----------------------------------------------------------------------------------
Texture2D  diffuseTexture : register( t0 );
SamplerState  diffuseTextureSampler : register( s0 );

//-----------------------------------------------------------------------------------
// VSInput structure
//-----------------------------------------------------------------------------------
struct  VSInput
{
		float4  position				: POSITION;
		float3  normal					: NORMAL;
		float2  texcoord				: TEXCOORD0;
};

//-----------------------------------------------------------------------------------
// PSInput structure
//-----------------------------------------------------------------------------------
struct  PSInput
{
		float4  position				: SV_POSITION;
		float3  normal					: NORMAL;
		float2  texcoord				: TEXCOORD0;
};


struct  PSOutput
{
		float2  normal : SV_Target0;
		float4  color : SV_Target1;
		float  specular : SV_Target2;
};


static  const  float  PI = 3.1415926535f;