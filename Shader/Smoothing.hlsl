#include  "Header\Structs.hlsli"

float3  NormalizedLambert( float3  diffuse, float3  lightDir, float3  normal )
{
		return diffuse * max( mul( normal, lightDir ), 0.0f ) * ( 1.0f / PI );
}

PSOutput  main( PSInput input )// : SV_Target
{
		PSOutput  OUT = ( PSOutput ) 0;

		//float  l = saturate( dot( input.normal,float3( 2, 5, -3 ) ) );
		float3  light = ( float3 )normalize( float4( 2, 5, -3, 1.0f ) );

		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.texcoord );

		float4  color = meshColor * texel;

		float3  nor = normalize( input.normal );
		float3  diffuse = NormalizedLambert( float3( 1.0f, 1.0f, 1.0f )*meshColor.rgb, light, nor );

		//return  texel;
		OUT.normal = input.normal;
		OUT.color = float4( diffuse*texel.rgb, 1.0f );
		OUT.specular = 0.2;

		return  OUT;
}