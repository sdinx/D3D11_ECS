#include "header/Structs.hlsli"

const uint MAX_LIGHT_COUNT = 1000;
const uint MAX_X_PLANE = 16;
const uint MAX_Y_PLANE = 16;
const uint MAX_Z_PLANE = 16;

[numthreads( MAX_LIGHT_COUNT, 1, 1 )]
void main( uint3 DTid : SV_DispatchThreadID )
{


	{

        int x = 0; // 一番左から
        do // ライトの左側の影響範囲内の x plane を特定
        {
            ++x; // 次の x plane に移動.
        } while ( x < MAX_X_PLANE && abs( x_planes[x] - light.pos ) >= light.radius );

        int x_max = MAX_X_PLANE; // 一番右から
        do // ライトの右側の影響範囲内の x plane を特定
        {
            --x_max;
        } while ( x_max >= x && abs( x_planes[x_max] - light.pos ) >= light.radius );

        for ( --x; x <= x_max; x++ )
            0; // ライトリストに追加
    } // end for

}

const uint NUM_SLICE_COUNT = 16;

[numthreads( MAX_LIGHT_COUNT, 1, 1 )]
void LightCulling( uint3 DTid : SV_DispatchThreadID )
{
    float2 fov = 2.0f * atan( 1.0f / g_proj._11_11 ) * 180.0f / PI;
    float tanFov = tan( fov.y * 0.5f );

    float h0 = ( 2 * near * tanFov ) / NUM_SLICE_COUNT;

    uint k = log( -z / near ) / log( 1 + ( 2 * tanFov / NUM_SLICE_COUNT ) );

}


void FrustumCulling()
{
    const uint a = NUM_SLICE_COUNT;
    uint sliceCount = NUM_SLICE_COUNT;
    float z = 0;

	// 指数関数的にフラスタムを分割
    for ( uint sliceIndex = 1; sliceIndex <= sliceCount; sliceIndex++ )
    {
        z = lerp( 0.0f, 1.0f, log( sliceIndex / sliceCount * ( a - 1 ) + 1 ) / log( a ) );
    } // end for
}