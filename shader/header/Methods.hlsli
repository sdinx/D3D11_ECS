//----------------------------------------------------------------------------------
// OctEncode methods
//----------------------------------------------------------------------------------
float2 OctWrap( float2 V )
{
    return ( 1.0 - abs( V.yx ) ) * ( V.xy >= 0.0 ? 1.0 : -1.0 );
}


float2 OctEncode( float3 N )
{
    N /= ( abs( N.x ) + abs( N.y ) + abs( N.z ) );
    N.xy = N.z >= 0.0 ? N.xy : OctWrap( N.xy );
    N.xy = N.xy * 0.5 + 0.5;
    return N.xy;
}


float3 OctDecode( float2 N )
{
    N = N * 2.0 - 1.0;

    float3 n;
    n.z = 1.0 - abs( N.x ) - abs( N.y );
    n.xy = n.z >= 0.0 ? N.xy : OctWrap( N.xy );
    n = normalize( n );
    return n;
}


//----------------------------------------------------------------------------------
// Shadow mapping methods
//----------------------------------------------------------------------------------
float VarianceShadowFilter( float2 tex, float fragDepth, float4 Depth )
{

}