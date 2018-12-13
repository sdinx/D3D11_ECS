#include "header/DeferredStruct.hlsli"
#include "header/Methods.hlsli"
#include "header/Structs.hlsli"


Texture2D<float2> texNormal : register( t0 );
Texture2D<float4> texDiffuse : register( t1 );
Texture2D<float> texSpecular : register( t2 );
Texture2D<float> texDepth : register( t3 );
Texture2D<uint> texStencil : register( t4 );
Texture3D<uint2> texCluster : register( t5 );

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



    return diff * IN.color;
    //return float4( nor, 1 );
}