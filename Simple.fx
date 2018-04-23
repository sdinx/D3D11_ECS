//-----------------------------------------------------------------------------------
// File : Simple.fx
// Desc : Shader For Simple Demo.
// Copyright(c) Project Asura. All right reserved.
//-----------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////////////
// CBPerFrame constant buffer
/////////////////////////////////////////////////////////////////////////////////////
cbuffer CBuffer : register( b0 )
{
		float4x4  View;   //!< ビュー行列.
		float4x4  Proj;   //!< 射影行列.
};

cbuffer CObject : register( b1 )
{
		float4x4  World;  //!< ワールド行列.
};

/////////////////////////////////////////////////////////////////////////////////////
// VSInput structure
/////////////////////////////////////////////////////////////////////////////////////
struct VSInput
{
    float3 Position : POSITION;     //!< 位置座標です.
};

/////////////////////////////////////////////////////////////////////////////////////
// GSPSInput structure
/////////////////////////////////////////////////////////////////////////////////////
struct GSPSInput
{
    float4 Position : SV_POSITION;  //!< 位置座標です.
};

//-----------------------------------------------------------------------------------
//! @brief      頂点シェーダエントリーポイントです.
//-----------------------------------------------------------------------------------
GSPSInput VSFunc( VSInput input )
{
    GSPSInput output = (GSPSInput)0;

    // 入力データをそのまま流す.
    output.Position = float4( input.Position, 1.0f );

    return output;
}

//-----------------------------------------------------------------------------------
//! @brief      ジオメトリシェーダエントリーポイントです.
//-----------------------------------------------------------------------------------
[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream )
{
    for( int i=0; i<3; ++i )
    {
        GSPSInput output = (GSPSInput)0;

        // ワールド空間に変換.
        float4 worldPos = mul( World, input[ i ].Position );

        // ビュー空間に変換.
        float4 viewPos  = mul( View,  worldPos );

        // 射影空間に変換.
        float4 projPos  = mul( Proj,  viewPos );

        // 出力値設定.
        output.Position = projPos;

        // ストリームに追加.
        stream.Append( output );
    }
    stream.RestartStrip();
}

//------------------------------------------------------------------------------------
//! @brief      ピクセルシェーダエントリーポイントです.
//------------------------------------------------------------------------------------
float4 PSFunc( GSPSInput output ) : SV_TARGET0
{
    return float4( 0.25f, 1.0f, 0.25f, 1.0f );
}
