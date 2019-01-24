#include "header/Structs.hlsli"
#include "header/ShaderDefines.h"

groupshared uint s_nLightCounts = 0;

uint PackCoords( uint2 coords )
{
    return coords.y << 16 | coords.x;
}

uint2 UnpackCoords( uint coords )
{
    return uint2( coords & 0xFFFF, coords >> 16 );
}

[numthreads( NUM_TILE_PER_PIXELS, NUM_TILE_PER_PIXELS, 1 )]
void main(
uint groupIndex : SV_GroupIndex,
uint3 dispatchThreadId : SV_DispatchThreadID,
uint3 groupThreadId : SV_GroupThreadID,
uint3 groupId : SV_GroupId
)
{

    const uint nTotalLights = NUM_POINT_LIGHT_COUNTS;
    float minTileZ = g_nearFarClip.x;
    float maxTileZ = g_nearFarClip.y;

	// タイルサイズとタイル位置を計算.
    float2 tileScale = float2( 799, 599 ) * rcp( float( 2 * NUM_TILE_PER_PIXELS ) );
    float2 tileBias = tileScale - float2( groupId.xy );

    float4 c1 = float4( g_proj._11 * tileScale.x, 0, tileBias.x, 0 );
    float4 c2 = float4( 0, -g_proj._22 * tileScale.y, tileBias.y, 0 );
    float4 c3 = float4( 0, 0, 1.0f, 0 );

	// タイルを作成.
    float4 frustumPlane[6];
    frustumPlane[0] = c3 - c1;
    frustumPlane[1] = c3 + c1;
    frustumPlane[2] = c3 - c2;
    frustumPlane[3] = c3 + c2;
    frustumPlane[4] = float4( 0, 0, 1.0f, -minTileZ );
    frustumPlane[5] = float4( 0, 0, -1.0f, maxTileZ );

	[unroll(4)]
    for ( uint i = 0; i < 4; i++ )
        frustumPlane[i] *= rcp( length( frustumPlane[i].xyz ) );
	
    float4 viewPos;
    float d;
    bool isInFrustum = true;
    for ( uint lightIndex = groupIndex; lightIndex < nTotalLights; lightIndex += NUM_TILE_GROUP_SIZE )
    {
        SPointLight light = g_ptLight[lightIndex];
        if ( light.distance == 0.0f )
            continue;
        viewPos = mul( g_view, float4( light.pos, 1.0f ) );
		
        isInFrustum = true;
		[unroll(6)]
        for ( uint i = 0; i < 6; i++ )
        {
            d = dot( frustumPlane[i], viewPos );
			// タイルに接触しているか.
            isInFrustum = isInFrustum && ( d >= -light.distance );
        }

        if ( isInFrustum )
        {
			// ライト数をカウント.
            InterlockedAdd( s_nLightCounts, 1 );
        }

    }
	
	// 他のスレッドがここに到達するまでブロック.
    GroupMemoryBarrierWithGroupSync();

    texCluster[dispatchThreadId] = uint2( s_nLightCounts, 0 );

}