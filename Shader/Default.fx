#include  "Header\Structs.hlsli"

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


		output.normal = normalize( mul( ( float3x3 )world, input.normal ) );
		output.texcoord = input.texcoord;

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
		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.texcoord );
		
		float4  color = meshColor * texel;

		return  color;
}