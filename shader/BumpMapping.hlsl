#include  "header/Structs.hlsli"
#include  "header/Methods.hlsli"

PSOutput dsad( PSInput IN )
{
    PSOutput OUT = (PSOutput) 0;

    float3 lDiffuse = g_pointLightDiffuse;
    float4 lSpecular = g_pointLightSpecular;
    float3 lAmbient = g_pointLightAmbient;
    float3 lPos = g_pointLightPos;
	
    float4 texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );
	

    float3 nor = normalize( IN.normal );
    float3 ambient = lAmbient * texel.rgb;
    float3 lightDir = normalize( lPos - IN.worldPos );

    float diff = max( dot( nor, lightDir ), 0.0 );
    float3 diffuse = lDiffuse * diff * texel.rgb;

	// specular
    float3 viewDir = normalize( g_cameraPos - IN.worldPos );
    float3 reflectDir = reflect( -lightDir, nor );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), 1.0 );
    float3 specular = lSpecular.rgb * ( spec * float3( 1, 1, 1 ) );

    float3 result = ambient + diffuse + specular;
	
    OUT.normal = OctEncode( IN.normal );
    OUT.color = float4( result, 1.0f );
    OUT.specular = g_specular.w;

    return OUT;
}


PSOutput main( PSInput IN )
{
    PSOutput OUT = (PSOutput) 0;

    float4 texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );
    float4 n = normalTexture.Load( int3( IN.texcoord, 0 ) );
    n = normalize( mul( g_world, n ) );
    n = normalize( n );
    float3 nor = IN.normal; // * nor.xyz;

    float3 viewDir = normalize( g_cameraPos.xyz - IN.worldPos );
    float3 l = g_pointLightPos - IN.worldPos;
    float d = length( l );
    float3 r = 2.0 * nor * dot( nor, l ) - l;
    float a = saturate( 1.0f / ( g_pointLightAttenuate.x + g_pointLightAttenuate.y * d + g_pointLightAttenuate.z * d * d ) );
    float3 iA = texel.rgb;
    float3 iD = saturate( dot( l, nor ) ) * texel.rgb * a;
    float3 iS = pow( saturate( dot( r, viewDir ) ), g_pointLightSpecular.w ) * a;
    float3 spec = float3( saturate( iA + iD + iS ) );

    OUT.normal = OctEncode( nor );
    OUT.color = float4( spec, 1.0f );
    OUT.specular = g_specular.w;

    return OUT;
}


PSInput OutlineVS( VSInput IN, out float4 out_line : TEXCOORD1 )
{
    PSInput OUT = (PSInput) 0;

	// ワールド空間に変換
    OUT.position = mul( g_world, IN.position );
    OUT.worldPos = OUT.position.xyz;

	// ビュー空間に変換
    OUT.position = mul( g_view, OUT.position );

	// 射影空間に変換
    OUT.position = mul( g_proj, OUT.position );

    OUT.normal = normalize( mul( (float3x3) g_world, IN.normal ) );

    OUT.texcoord = IN.texcoord;

    float4 original = mul( mul( mul( IN.position, g_world ), g_view ), g_proj );

    out_line =

    return OUT;
}


PSOutput CelShading( PSInput IN )
{
    PSOutput OUT = (PSOutput) 0;

    float4 texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );
    float4 n = normalTexture.Load( int3( IN.texcoord, 0 ) );
    n = normalize( mul( g_world, n ) );
    n = normalize( n );
    float3 nor = IN.normal; // * nor.xyz;

    float3 viewDir = normalize( g_cameraPos.xyz - IN.worldPos );
    float3 l = g_pointLightPos - IN.worldPos;
    float d = length( l );
    float3 r = 2.0 * nor * dot( nor, l ) - l;
    float a = saturate( 1.0f / ( g_pointLightAttenuate.x + g_pointLightAttenuate.y * d + g_pointLightAttenuate.z * d * d ) );
    float3 iA = texel.rgb;
    float3 iD = saturate( dot( l, nor ) ) * texel.rgb * a;
    float3 iS = pow( saturate( dot( r, viewDir ) ), g_pointLightSpecular.w ) * a;
    float3 spec = float3( saturate( iA + iD + iS ) );

    OUT.normal = OctEncode( nor );
    OUT.color = float4( spec, 1.0f );
    OUT.specular = g_specular.w;

    return OUT;
}