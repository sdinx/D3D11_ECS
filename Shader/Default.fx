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
//! @brief      ���_�V�F�[�_�G���g���[�|�C���g
//-----------------------------------------------------------------------------------
GSPSInput VSFunc( VSInput input )
{
		GSPSInput output = ( GSPSInput ) 0;

		// ���[���h��Ԃɕϊ�
		output.position = mul( world, input.position );

		// �r���[��Ԃɕϊ�
		output.position = mul( view, output.position );

		// �ˉe��Ԃɕϊ�
		output.position = mul( proj, output.position );

		return output;
}

//-----------------------------------------------------------------------------------
//! @brief      �W�I���g���V�F�[�_�G���g���[�|�C���g
//-----------------------------------------------------------------------------------
[maxvertexcount( 3 )]
void GSFunc( triangle  GSPSInput  input[3], inout  TriangleStream<GSPSInput>  stream )
{
		GSPSInput output;

		for ( int i = 0; i < 3; ++i )
		{
				output = ( GSPSInput ) 0;

				// ���[���h��Ԃɕϊ�
				output.position = mul( world, input[i].position );

				// �r���[��Ԃɕϊ�
				output.position = mul( view, output.position );

				// �ˉe��Ԃɕϊ�
				output.position = mul( proj, output.position );

				// �X�g���[���ɒǉ�
				stream.Append( output );
		}
		stream.RestartStrip();
}

//------------------------------------------------------------------------------------
//! @brief      �s�N�Z���V�F�[�_�G���g���[�|�C���g
//------------------------------------------------------------------------------------
float4 PSFunc( GSPSInput input ) : SV_TARGET
{
		//float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.Tex );

		return  meshColor;
}