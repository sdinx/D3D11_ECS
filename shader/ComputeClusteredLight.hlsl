#include "header/Structs.hlsli"

const int NUM_LIGHT_COUNT = 1000;
const int NUM_TILE_PER_PIXELS = 32;
const int NUM_X_TILES = 40;
const int NUM_Y_TILES = 30;
const int NUM_DEPTH_SLICES = 16;

[numthreads( MAX_LIGHT_COUNT, 1, 1 )]
void test( uint3 DTid : SV_DispatchThreadID )
{

	{

        int x = 0; // 一番左から
        do // ライトの左側の影響範囲内の x plane を特定
        {
            ++x; // 次の x plane に移動.
        } while ( x < NUM_X_TILES && abs( x_planes[x] - light.pos ) >= light.radius );

        int x_max = NUM_X_TILES; // 一番右から
        do // ライトの右側の影響範囲内の x plane を特定
        {
            --x_max;
        } while ( x_max >= x && abs( x_planes[x_max] - light.pos ) >= light.radius );

        for ( --x; x <= x_max; x++ )
            0; // ライトリストに追加
    } // end for

}


[numthreads( MAX_LIGHT_COUNT, 1, 1 )]
void LightCulling( uint3 DTid : SV_DispatchThreadID )
{
    float2 fov = 2.0f * atan( 1.0f / g_proj._22 ) * 180.0f / PI;
    float tanFov = tan( fov.y * 0.5f );

    float h0 = ( 2 * near * tanFov ) / NUM_DEPTH_SLICES;

    uint k = log( -z / near ) / log( 1 + ( 2 * tanFov / NUM_DEPTH_SLICES ) );

}


void FrustumCulling()
{
    const uint a = NUM_DEPTH_SLICES;
    float z = 0;

	// 指数関数的にフラスタムを分割
    for ( uint sliceIndex = 1; sliceIndex <= NUM_DEPTH_SLICES; sliceIndex++ )
    {
        z = lerp( 0.1f, 1000.0f, log( sliceIndex / NUM_DEPTH_SLICES * ( a - 1 ) + 1 ) / log( a ) );
    } // end for
}


[numthreads( NUM_X_TILES, NUM_Y_TILES, 1 )]
void main( uint3 thread_id : SV_DispatchThreadID )
{

}