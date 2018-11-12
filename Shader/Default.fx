#include  "Header\Structs.hlsli"
#include  "Header\Methods.hlsli"

//-----------------------------------------------------------------------------------
//! @brief      頂点シェーダエントリーポイント
//-----------------------------------------------------------------------------------
PSInput VSFunc( VSInput IN )
{
		PSInput OUT = ( PSInput ) 0;

		// ワールド空間に変換
		OUT.position = mul( g_world, IN.position );

		// ビュー空間に変換
		OUT.position = mul( g_view, OUT.position );

		// 射影空間に変換
		OUT.position = mul( g_proj, OUT.position );

		OUT.normal = normalize( mul( ( float3x3 )g_world, IN.normal ) );
		OUT.texcoord = IN.texcoord;

		return OUT;
}

//-----------------------------------------------------------------------------------
//! @brief      ジオメトリシェーダエントリーポイント
//-----------------------------------------------------------------------------------
[maxvertexcount( 3 )]
void GSFunc( triangle  PSInput  input[3], inout  TriangleStream<PSInput>  stream )
{
		PSInput output;

		for ( int i = 0; i < 3; ++i )
		{
				output = ( PSInput ) 0;

				// ワールド空間に変換
				output.position = mul( g_world, input[i].position );

				// ビュー空間に変換
				output.position = mul( g_view, output.position );

				// 射影空間に変換
				output.position = mul( g_proj, output.position );

				// ストリームに追加
				stream.Append( output );
		}
		stream.RestartStrip();
}

//------------------------------------------------------------------------------------
//! @brief      ピクセルシェーダエントリーポイント
//------------------------------------------------------------------------------------
PSOutput PSFunc( PSInput IN )// : SV_Target
{
		PSOutput  OUT = ( PSOutput ) 0;

		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );

		OUT.normal = OctEncode( IN.normal );
		OUT.color = g_diffuse * texel;
		OUT.specular = g_specular.w;

		return  OUT;
}