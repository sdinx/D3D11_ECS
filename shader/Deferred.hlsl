#include  "header/DeferredStruct.hlsli"

Texture2D<float3>  texNormal : register( t0 );
SamplerState  ssNormal : register( s0 );
Texture2D<float4>  texDiffuse : register( t1 );
SamplerState  ssDiffuse : register( s1 );
Texture2D<float>  texSpecular : register( t2 );
SamplerState  ssSpecular : register( s2 );
Texture2D<float>  texDepth : register( t3 );
SamplerState  ssDepth : register( s3 );
Texture2D<float>  texStencil : register( t4 );
SamplerState  ssStencil : register( s4 );

PSInput  vsmain( VSInput  IN )
{
		PSInput  OUT = ( PSInput ) 0;

		OUT.position = float4( IN.position, 1 );
		OUT.texcoord = IN.texcoord;
		OUT.color = IN.color;

		return  OUT;
}


float4  psmain( PSInput  IN ) : SV_Target
{
		static  uint  test = 0;
		float4  diff = texDiffuse.Sample( ssDiffuse, IN.texcoord );
		//return  float4( 0.5, 0.5, 0.5, 1 );
		return  diff * IN.color;
}