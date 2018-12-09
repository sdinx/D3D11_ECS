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
    uint index : SV_RenderTargetArrayIndex;
};


struct LightPSOutput
{
    uint2 cluster : SV_Target;
};


LightPSInput vsmain( LightVSInput IN, in uint instanceId : SV_InstanceID )
{
    LightPSInput OUT = (LightPSInput) 0;

	// ワールド空間に変換
    OUT.position = float4( ptLights[instanceId].pos + IN.position.xyz, IN.position.w );

	// ビュー空間に変換
    OUT.position = mul( g_view, OUT.position );

	// 射影空間に変換
    OUT.position = mul( g_proj, OUT.position );

    OUT.index = instanceId;

    return OUT;
}


[maxvertexcount( 3 )]
void gsmain( triangle LightVSInput IN[3], inout TriangleStream<LightPSInput> triOutputStream )
{
    LightPSInput OUT = (LightPSInput) 0;
	
    for ( int i = 0; i < 3; ++i )
    {
        OUT.position = IN[i].position;
        OUT.index = 5;

        triOutputStream.Append( OUT );
    }

}


LightPSOutput psmain( LightPSInput IN )
{
    LightPSOutput OUT = (LightPSOutput) 0;

    OUT.cluster = uint2( 100, 100 );

    return OUT;
}