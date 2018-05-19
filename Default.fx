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
Texture2D  diffuseTexture : register( t0 );
SamplerState  diffuseTextureSampler : register( s0 );

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
//! @brief      ���_�V�F�[�_�G���g���[�|�C���g
//-----------------------------------------------------------------------------------
GSPSInput VSFunc( VSInput input )
{
		GSPSInput output = ( GSPSInput ) 0;

		// ���̓f�[�^�����̂܂ܗ���
		output.Position = float4( input.Position, 1.0f );
		//output.Tex = input.Tex;

		return output;
}

//-----------------------------------------------------------------------------------
//! @brief      �W�I���g���V�F�[�_�G���g���[�|�C���g
//-----------------------------------------------------------------------------------
[maxvertexcount( 6 )]
void GSFunc( triangle GSPSInput input[3], inout TriangleStream<GSPSInput> stream )
{
		GSPSInput output;
		float4 worldPos, viewPos, projPos;

		for ( int i = 0; i<3; ++i )
		{
				output = ( GSPSInput ) 0;

				// ���[���h��Ԃɕϊ�
				worldPos = mul( World, input[i].Position );

				// �r���[��Ԃɕϊ�
				viewPos = mul( View, worldPos );

				// �ˉe��Ԃɕϊ�
				projPos = mul( Proj, viewPos );

				// �o�͒l�ݒ�
				output.Position = projPos;

				// �X�g���[���ɒǉ�
				stream.Append( output );
		}
		stream.RestartStrip();
}

//------------------------------------------------------------------------------------
//! @brief      �s�N�Z���V�F�[�_�G���g���[�|�C���g
//------------------------------------------------------------------------------------
float4 PSFunc( GSPSInput input ) : SV_TARGET0
{
		//float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.Tex );

		return  meshColor;
}