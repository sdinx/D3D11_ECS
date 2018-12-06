#include "header/DeferredStruct.hlsli"
#include "header/Methods.hlsli"
#include "header/Structs.hlsli"


Texture2D<float2> texNormal : register( t0 );
Texture2D<float4> texDiffuse : register( t1 );
Texture2D<float> texSpecular : register( t2 );
Texture2D<float> texDepth : register( t3 );
Texture2D<uint> texStencil : register( t4 );
Texture2D<uint2> texCluster : register( t5 );

SamplerState samState : register( s0 );
//RWStructuredBuffer<uint> lightIndices : register( t6 );


PSDeferredInput vsmain( VSDeferredInput IN )
{
    PSDeferredInput OUT = (PSDeferredInput) 0;

    OUT.position = float4( IN.position, 1 );
    OUT.texcoord = IN.texcoord;
    OUT.color = IN.color;

    return OUT;
}


float4 psmain( PSDeferredInput IN ) : SV_Target
{
    static uint test = 0;
    float4 diff = texDiffuse.Sample( samState, IN.texcoord );

	// ñ@ê¸ÇÃïúå≥
    float2 encN = texNormal.Sample( samState, IN.texcoord ).xy;
    float3 nor = OctDecode( encN );

    //float depth = texDepth.Sample( ssDepth, IN.texcoord );
    //float4 projPos = float4( IN.texcoord * 2.0 - float2( 1, 1 ), depth, 1.0 );
    //float4 pos = mul( projPos, invView );
    //float3 resPos = pos.xyz / pos.w;

    //float4 lightPos = float4( 3, 10, -3, 1 );
    //float3 lightDir = normalize( lightPos.xyz - resPos );
    //float3 viewDir = normalize( viewPos - resPos );
    //float3 halfwayDir = normalize( lightDir + viewDir );
    //float spec = pow( max( dot( nor, halfwayDir ), 0.0 ), shine );
    //float3 specular = lightColor * spec;

    //float4 lightDir = float4( 2, 50, -3, 1 );
    //lightDir = normalize( lightDir );
    //float lightIntensity = saturate( dot( nor.xyz, lightDir ) );
    //return saturate( diff * lightIntensity );

    return diff * IN.color;
    //return float4( nor, 1 );
}