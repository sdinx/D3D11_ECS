#include  "header/DeferredStruct.hlsli"

Texture2D<float3>  texNormal : register( t0 );
SamplerState  ssNormal : register( s0 );
Texture2D<float4>  texDiffuse : register( t1 );
SamplerState  ssDiffuse : register( s1 );
Texture2D<float>  texSpecular : register( t2 );
SamplerState  ssSpecular : register( s2 );

PSInput  vsmain( VSInput  IN )
{
		PSInput  OUT = ( PSInput ) 0;

		OUT.position = IN.position;
		OUT.texcoord = IN.texcoord;

		return  OUT;
}


float4  psmain( PSInput  IN ) : SV_TARGET
{
		float4  diff = texDiffuse.Sample( ssDiffuse, 2 * ( IN.texcoord - float2( 0,0.5 ) ) );
		return  float4( 0.5, 0.5, 0.5, 1 );
		return  diff;
}