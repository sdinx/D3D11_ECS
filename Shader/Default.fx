//-----------------------------------------------------------------------------------
// CBPerFrame constant buffer
//-----------------------------------------------------------------------------------
cbuffer CBuffer : register( b0 )
{
		float4x4  view;
		float4x4  proj;
};

cbuffer CObject : register( b1 )
{
		float4x4  world;
		float4  meshColor;
};

//-----------------------------------------------------------------------------------
// Texture variables
//-----------------------------------------------------------------------------------
Texture2D  diffuseTexture : register( t0 );
SamplerState  diffuseTextureSampler : register( s0 );

//-----------------------------------------------------------------------------------
// VSInput structure
//-----------------------------------------------------------------------------------
struct VSInput
{
		float4  position : POSITION;
		//float2  texcoord : TEXCOORD0;
};

//-----------------------------------------------------------------------------------
// GSPSInput structure
//-----------------------------------------------------------------------------------
struct GSPSInput
{
		float4  position : SV_POSITION;
		//float2  texcoord : TEXCOORD0;
};

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
		//float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.Tex );

		return  meshColor;
}