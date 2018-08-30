#include  "Header\Structs.hlsli"

float3  NormalizedLambert( float3  diffuse, float3  lightDir, float3  normal )
{
		return diffuse * max( dot( normal, lightDir ), 0.0f ) * ( 1.0f / PI );
}

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
		float3  light = normalize( float4( 2, 5, -3, 1.0f ) );

		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.texcoord );
		
		float4  color = meshColor * texel;
		
		float3  nor = normalize( input.normal );
		float3  diffuse = NormalizedLambert( float4( 0.5f, 0.5f, 0.5f, 1.0f )*texel.rgb, light, nor );

		//return  color;
		return  float4( diffuse, 1.0f );
}