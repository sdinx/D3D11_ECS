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
		float4x4  View;   //!< �r���[�s��.
		float4x4  Proj;   //!< �ˉe�s��.
};

cbuffer CObject : register( b1 )
{
		float4x4  World;  //!< ���[���h�s��.
};

/////////////////////////////////////////////////////////////////////////////////////
// VSInput structure
/////////////////////////////////////////////////////////////////////////////////////
struct VSInput
{
    float3 Position : POSITION;     //!< �ʒu���W�ł�.
};

/////////////////////////////////////////////////////////////////////////////////////
// GSPSInput structure
/////////////////////////////////////////////////////////////////////////////////////
struct GSPSInput
{
    float4 Position : SV_POSITION;  //!< �ʒu���W�ł�.
};

//-----------------------------------------------------------------------------------
//! @brief      ���_�V�F�[�_�G���g���[�|�C���g�ł�.
//-----------------------------------------------------------------------------------
GSPSInput VSFunc( VSInput input )
{
    GSPSInput output = (GSPSInput)0;

    // ���̓f�[�^�����̂܂ܗ���.
    output.Position = float4( input.Position, 1.0f );

    return output;
}

//-----------------------------------------------------------------------------------
//! @brief      �W�I���g���V�F�[�_�G���g���[�|�C���g�ł�.
//-----------------------------------------------------------------------------------
[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream )
{
    for( int i=0; i<3; ++i )
    {
        GSPSInput output = (GSPSInput)0;

        // ���[���h��Ԃɕϊ�.
        float4 worldPos = mul( World, input[ i ].Position );

        // �r���[��Ԃɕϊ�.
        float4 viewPos  = mul( View,  worldPos );

        // �ˉe��Ԃɕϊ�.
        float4 projPos  = mul( Proj,  viewPos );

        // �o�͒l�ݒ�.
        output.Position = projPos;

        // �X�g���[���ɒǉ�.
        stream.Append( output );
    }
    stream.RestartStrip();
}

//------------------------------------------------------------------------------------
//! @brief      �s�N�Z���V�F�[�_�G���g���[�|�C���g�ł�.
//------------------------------------------------------------------------------------
float4 PSFunc( GSPSInput output ) : SV_TARGET0
{
    return float4( 0.25f, 1.0f, 0.25f, 1.0f );
}
