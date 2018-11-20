#include  "header/DeferredStruct.hlsli"
#include  "header/Structs.hlsli"

struct PointLight
{
    float3 position;
    float3 diffuse;
    float3 specular;
    float3 attenuate;
};

PointLight lights[10];

Texture2D<float2> texNormal : register( t0 );
SamplerState ssNormal : register( s0 );
Texture2D<float4> texDiffuse : register( t1 );
SamplerState ssDiffuse : register( s1 );
Texture2D<float> texSpecular : register( t2 );
SamplerState ssSpecular : register( s2 );
Texture2D<float> texDepth : register( t3 );
SamplerState ssDepth : register( s3 );
Texture2D<uint> texStencil : register( t4 );
SamplerState ssStencil : register( s4 );

Texture3D<uint2> texCluster : register( t5 ); // クラスタマップ ( R32 : offset, G32 : [PointLightCount, SpotLightCount] )
Texture1D<uint> texLightIndex : register( t6 );

float4 main( PSDeferredInput IN ) : SV_Target
{
	// スクリーン上の位置からデプス値を取得.
    int3 texcoord = int3( IN.position.xy, 0 );
    float depth = texDepth.Load( texcoord );
	
    float2 fov = 2.0 * atan( 1.0 / g_proj._11_11 ) * 180.0 / PI;

	// 深さからクラスタ位置を取得.
    int slice = int( max( log2( depth * zParam.x, zParam.y ) * scale + bias, 0 ) );
    int4 clusterCoord = int4( texcoord >> 6, slice, 0 ); // tile size 64
	
	// クラスタを取得してライトリストのインデックスを取り出す.
    uint2 cluster = texCluster.Load( clusterCoord );
    uint lightIndex = cluster.x;

	// ポイントライト・スポットライトの数をそれぞれ取得.
    const uint pointLightCount = cluster.y & 0xFFFF;
    const uint spotLightCount = cluster.y >> 16;
	
	// クラスタ内をポイントライティング
    for ( uint pl = 0; pl < pointLightCount; pl++ )
    {
		// ライトリストからライト情報のインデックスを取得.
        uint index = lightLists[lightIndex++].x;
		
		// ライト情報.
        float3 lightPos = lights[index].position;
        float3 color = lights[index].diffuse;
		
		// ライティング処理...
    }
	
	// クラスタ内をスポットライティング
    //for ( uint sl = 0; sl < spotLightCount; sl++ )
    //{
	//
    //}
    return float4( 1.0f, 1.0f, 1.0f, 1.0f );
}