#include  "header/Structs.hlsli"
#include  "header/Methods.hlsli"

PSOutput dsad( PSInput IN )
{
    PSOutput OUT = (PSOutput) 0;

    float3 lDiffuse = float3( 1, 1, 1 );
    float4 lSpecular = float4( 5, 5, 5, 5 );
    float3 lAmbient = float3( 1, 1, 1 );
    float3 lPos = float3( 0, 0, 0.5 );
	
    float4 texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );
	

    float3 nor = normalize( IN.normal );
    float3 ambient = lAmbient * texel.rgb;
    float3 lightDir = normalize( lPos - IN.position.rgb );

    float diff = max( dot( nor, lightDir ), 0.0 );
    float3 diffuse = lDiffuse * diff * texel.rgb;

	// specular
    float3 viewDir = normalize( g_cameraPos - IN.position.rgb );
    float3 reflectDir = reflect( -lightDir, nor );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), 1.0 );
    float3 specular = lSpecular.rgb * ( spec * float3( 1, 1, 1 ) );

    float3 result = ambient * diffuse * specular;
	
    OUT.normal = OctEncode( IN.normal );
    OUT.color = float4( result, 1.0f );
    OUT.specular = g_specular.w;

    return OUT;
}


PSOutput main( PSInput IN )
{
    PSOutput OUT = (PSOutput) 0;

    return OUT;
}