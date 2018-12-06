#include "header/Structs.hlsli"

//-----------------------------------------------------------------------------------
// Light structure
//-----------------------------------------------------------------------------------
struct LightVSInput
{
    float4 position : POSITION;
};


struct LightPSInput
{
    float4 position : SV_POSITION;
};


struct LightPSOutput
{
    uint2 cluster : SV_Target;
};


LightPSInput vsmain( LightVSInput IN )
{
    LightPSInput OUT = (LightPSInput) 0;

	// ワールド空間に変換
    OUT.position = mul( g_world, IN.position );

	// ビュー空間に変換
    OUT.position = mul( g_view, OUT.position );

	// 射影空間に変換
    OUT.position = mul( g_proj, OUT.position );

    return OUT;
}


LightPSOutput psmain( LightPSInput IN )
{
    LightPSOutput OUT = (LightPSOutput) 0;

    OUT.cluster = uint2( 100, 100 );

    return OUT;
}