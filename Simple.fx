//-----------------------------------------------------------------------------------
// CBPerFrame constant buffer
//-----------------------------------------------------------------------------------
cbuffer CBuffer : register( b0 )
{
		float4x4  View;
		float4x4  Proj;
};

cbuffer CObject : register( b1 )
{
		float4x4  World;
		float4  meshColor;
};

//-----------------------------------------------------------------------------------
// Texture variables
//-----------------------------------------------------------------------------------
Texture2D  diffuseTexture;
SamplerState  diffuseTextureSampler;

//-----------------------------------------------------------------------------------
// VSInput structure
//-----------------------------------------------------------------------------------
struct VSInput
{
    float3  Position : POSITION;
		//float2  Tex : TEXCOORD0;
};

//-----------------------------------------------------------------------------------
// GSPSInput structure
//-----------------------------------------------------------------------------------
struct GSPSInput
{
    float4  Position : SV_POSITION;
		//float2  Tex : TEXCOORD0;
};

//-----------------------------------------------------------------------------------
//! @brief      頂点シェーダエントリーポイント
//-----------------------------------------------------------------------------------
GSPSInput VSFunc( VSInput input )
{
    GSPSInput output = (GSPSInput)0;

    // 入力データをそのまま流す
    output.Position = float4( input.Position, 1.0f );
		//output.Tex = input.Tex;

    return output;
}

//-----------------------------------------------------------------------------------
//! @brief      ジオメトリシェーダエントリーポイント
//-----------------------------------------------------------------------------------
[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream )
{
		GSPSInput output;

    for( int i=0; i<3; ++i )
    {
        output = (GSPSInput)0;

        // ワールド空間に変換
        float4 worldPos = mul( World, input[ i ].Position );

        // ビュー空間に変換
        float4 viewPos  = mul( View,  worldPos );

        // 射影空間に変換
        float4 projPos  = mul( Proj,  viewPos );

        // 出力値設定
        output.Position = projPos;

        // ストリームに追加
        stream.Append( output );
    }
    stream.RestartStrip();
}

//------------------------------------------------------------------------------------
//! @brief      ピクセルシェーダエントリーポイント
//------------------------------------------------------------------------------------
float4 PSFunc( GSPSInput input ) : SV_TARGET0
{
		//float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.Tex );

		return  meshColor;// *texel;
}
