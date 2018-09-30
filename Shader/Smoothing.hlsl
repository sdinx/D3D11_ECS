#include  "Header\Structs.hlsli"

float3  NormalizedLambert( float3  diffuse, float3  lightDir, float3  normal )
{
		return diffuse * max( mul( normal, lightDir ), 0.0f ) * ( 1.0f / PI );
}

float4  main( GSPSInput input ) : SV_TARGET
{
		//float  l = saturate( dot( input.normal,float3( 2, 5, -3 ) ) );
		float3  light = (float3)normalize( float4( 2, 5, -3, 1.0f ) );
		
		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.texcoord );
		
		float4  color = meshColor * texel;
		
		float3  nor = normalize( input.normal );
		float3  diffuse = NormalizedLambert( float3( 0.5f, 0.5f, 0.5f )*meshColor.rgb, light, nor );
		
		//return  texel;
		return  float4( diffuse*texel.rgb, 1.0f );
}