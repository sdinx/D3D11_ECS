#include  "header/Structs.hlsli"
#include  "header/Methods.hlsli"

float3  NormalizedLambert( float3  diffuse, float3  lightDir, float3  normal )
{
		return diffuse * max( mul( normal, lightDir ), 0.0f ) * ( 1.0f / PI );
}

PSOutput  main( PSInput IN )// : SV_Target
{
		PSOutput  OUT = ( PSOutput ) 0;

		//float  l = saturate( dot( input.normal,float3( 2, 5, -3 ) ) );
		float3  light = ( float3 )normalize( float4( 2, 5, -3, 1) );

		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );

		float4  color = g_diffuse * texel;

		float3  nor = normalize( IN.normal );
		float3  diffuse = NormalizedLambert( color.rgb, light, nor );

		//return  texel;
		OUT.normal = OctEncode( IN.normal );
		OUT.color = float4( diffuse, 1.0f );
		OUT.color = color;
		OUT.specular = g_specular.w;

		return  OUT;
}