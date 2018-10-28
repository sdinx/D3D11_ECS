Texture2D  texDepth : register( t0 );
Texture2D  texStencil : register( t0 );
Texture2D  texNormal : register( t0 );
Texture2D  texDiffuse : register( t0 );
Texture2D  texSpecular : register( t0 );

struct  VS_OUTPUT
{
		float4  position : SV_Position;
		float3  normal : NORMAL;
		float  depth : DEPTH_VIEW_SPACE;
};

struct  PS_OUTPUT
{
		float  depth : SV_Target0;
		float3  normal : SV_Target1;
		float4  diffuse : SV_Target2;
		float  specular : SV_Target3;
};

PS_OUTPUT  main( VS_OUTPUT  IN )
{
		PS_OUTPUT  OUT = ( PS_OUTPUT ) 0;
		OUT.normal = OctEncode( normalize( IN.normal ) );
		const  float2  texcoord = float2( 0.0f, 0.0f );

}