#include  "Header\Structs.hlsli"
#include  "Header\Methods.hlsli"

//-----------------------------------------------------------------------------------
//! @brief      頂点シェーダエントリーポイント
//-----------------------------------------------------------------------------------
PSInput VSFunc( VSInput input )
{
		PSInput output = ( PSInput ) 0;

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
void GSFunc( triangle  PSInput  input[3], inout  TriangleStream<PSInput>  stream )
{
		PSInput output;

		for ( int i = 0; i < 3; ++i )
		{
				output = ( PSInput ) 0;

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
PSOutput PSFunc( PSInput IN ) : SV_TARGET
{
		PSOutput  OUT = ( PSOutput ) 0;

		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );

		OUT.normal = OctEncode( IN.normal );
		OUT.color = meshColor * texel;
		OUT.specular = 0.2;

		return  OUT;
}