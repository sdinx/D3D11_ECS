//-----------------------------------------------------------------------------------
// CBPerFrame constant buffer
//-----------------------------------------------------------------------------------
cbuffer CBuffer : register( b0 )
{
    float4x4 g_view;
    float4x4 g_proj;
    float4x4 g_invView;
    float4x4 g_invProjection;
    float3 g_cameraPos;
};

cbuffer CObject : register( b1 )
{
    float4x4 g_world;
};

cbuffer CMaterial : register( b2 )
{
    float3 g_ambient;
    float3 g_diffuse;
    float4 g_emissive;
    float4 g_specular;
}

cbuffer CDirectionLight : register( b3 )
{
    float3 g_dirLightDirection;
}

cbuffer CPointLight : register( b4 )
{
    float3 g_pointLightPos;
    float3 g_pointLightAttenuate;
}

cbuffer CSpotLight : register( b5 )
{
    float3 g_spotLightPos;
    float3 g_spotLightAttenuate;
}

//-----------------------------------------------------------------------------------
// Texture buffers
//-----------------------------------------------------------------------------------
Texture2D diffuseTexture : register( t0 );
SamplerState diffuseTextureSampler : register( s0 );
Texture2D normalTexture : register( t1 );
SamplerState normalTextureSampler : register( s1 );


//-----------------------------------------------------------------------------------
// Structured buffers
//-----------------------------------------------------------------------------------
struct SPointLight
{
    float3 pos;
    float3 attenuate;
};
StructuredBuffer<SPointLight> ptLights : register( t3 );

//-----------------------------------------------------------------------------------
// VSInput structure
//-----------------------------------------------------------------------------------
struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
};

//-----------------------------------------------------------------------------------
// PSInput structure
//-----------------------------------------------------------------------------------
struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
};


struct PSOutput
{
    float2 normal : SV_Target0;
    float4 color : SV_Target1;
    float specular : SV_Target2;
};


static const float PI = 3.1415926535f;