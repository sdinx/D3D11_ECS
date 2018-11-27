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
    float4 g_ambient;
    float4 g_diffuse;
    float4 g_emissive;
    float4 g_specular;
};

cbuffer CDirectionLight : register( b2 )
{
    float3 g_dirLightDirection;
    float3 g_dirLightAmbient;
    float3 g_dirLightDiffuse;
    float4 g_dirLightSpecular;
}

cbuffer CPointLight : register( b3 )
{
    float3 g_pointLightPos;
    float3 g_pointLightAmbient;
    float3 g_pointLightDiffuse;
    float4 g_pointLightSpecular;
    float3 g_pointLightAttenuate;
}

cbuffer CSpotLight : register( b4 )
{
    float3 g_spotLightPos;
    float3 g_spotLightAmbient;
    float3 g_spotLightDiffuse;
    float4 g_spotLightSpecular;
    float3 g_spotLightAttenuate;
}

//-----------------------------------------------------------------------------------
// Texture variables
//-----------------------------------------------------------------------------------
Texture2D diffuseTexture : register( t0 );
SamplerState diffuseTextureSampler : register( s0 );
Texture2D normalTexture : register( t1 );
SamplerState normalTextureSampler : register( s1 );

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