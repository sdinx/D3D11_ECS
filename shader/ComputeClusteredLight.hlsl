#include "header/Structs.hlsli"

const int MAX_LIGHT_COUNT = 1000;
const int MAX_X_PLANE = 16;
const int MAX_Y_PLANE = 16;
const int MAX_Z_PLANE = 16;
const int NUM_SLICE_COUNT = 16;
const int NUM_TILE_GROUP_PIXELS = 16;

[numthreads( MAX_LIGHT_COUNT, 1, 1 )]
void main( uint3 DTid : SV_DispatchThreadID )
{


	{

        int x = 0; // ��ԍ�����
        do // ���C�g�̍����̉e���͈͓��� x plane �����
        {
            ++x; // ���� x plane �Ɉړ�.
        } while ( x < MAX_X_PLANE && abs( x_planes[x] - light.pos ) >= light.radius );

        int x_max = MAX_X_PLANE; // ��ԉE����
        do // ���C�g�̉E���̉e���͈͓��� x plane �����
        {
            --x_max;
        } while ( x_max >= x && abs( x_planes[x_max] - light.pos ) >= light.radius );

        for ( --x; x <= x_max; x++ )
            0; // ���C�g���X�g�ɒǉ�
    } // end for

}


[numthreads( MAX_LIGHT_COUNT, 1, 1 )]
void LightCulling( uint3 DTid : SV_DispatchThreadID )
{
    float2 fov = 2.0f * atan( 1.0f / g_proj._11_11 ) * 180.0f / PI;
    float tanFov = tan( fov.y * 0.5f );

    float h0 = ( 2 * near * tanFov ) / NUM_SLICE_COUNT;

    uint k = log( -z / near ) / log( 1 + ( 2 * tanFov / NUM_SLICE_COUNT ) );

}


void ComputeCluster()
{
    const uint a = NUM_SLICE_COUNT;
    uint sliceCount = NUM_SLICE_COUNT;
    float z = 0;
    int k = 0;

	// �w���֐��I�Ƀt���X�^���𕪊�
    for ( uint sliceIndex = 1; sliceIndex <= sliceCount; sliceIndex++ )
    {
        z = lerp( 0.0f, 1.0f, log( sliceIndex / sliceCount * ( a - 1 ) + 1 ) / log( a ) );

    } // end for

}


void FrustumCulling()
{
    const uint a = NUM_SLICE_COUNT;
    uint sliceCount = NUM_SLICE_COUNT;
    float z = 0;

	// �w���֐��I�Ƀt���X�^���𕪊�
    for ( uint sliceIndex = 1; sliceIndex <= sliceCount; sliceIndex++ )
    {
        z = lerp( 0.0f, 1.0f, log( sliceIndex / sliceCount * ( a - 1 ) + 1 ) / log( a ) );
    } // end for
}