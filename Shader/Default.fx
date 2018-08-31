#include  "Header\Structs.hlsli"

//-----------------------------------------------------------------------------------
//! @brief      頂点シェーダエントリーポイント
//-----------------------------------------------------------------------------------
GSPSInput VSFunc( VSInput input )
{
		GSPSInput output = ( GSPSInput ) 0;

		// ワールド空間に変換
		output.position = mul( world, input.position );

		// ビュー空間に変換
		output.position = mul( view, output.position );

		// 射影空間に変換
		output.position = mul( proj, output.position );


		output.normal = normalize( mul( ( float3x3 )world, input.normal ) );
		output.texcoord = input.texcoord;

		return output;
}

//-----------------------------------------------------------------------------------
//! @brief      ジオメトリシェーダエントリーポイント
//-----------------------------------------------------------------------------------
[maxvertexcount( 3 )]
void GSFunc( triangle  GSPSInput  input[3], inout  TriangleStream<GSPSInput>  stream )
{
		GSPSInput output;

		for ( int i = 0; i < 3; ++i )
		{
				output = ( GSPSInput ) 0;

				// ワールド空間に変換
				output.position = mul( world, input[i].position );

				// ビュー空間に変換
				output.position = mul( view, output.position );

				// 射影空間に変換
				output.position = mul( proj, output.position );

				// ストリームに追加
				stream.Append( output );
		}
		stream.RestartStrip();
}

//------------------------------------------------------------------------------------
//! @brief      ピクセルシェーダエントリーポイント
//------------------------------------------------------------------------------------
float4 PSFunc( GSPSInput input ) : SV_TARGET
{
		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.texcoord );
		
		float4  color = meshColor * texel;

		return  color;
}