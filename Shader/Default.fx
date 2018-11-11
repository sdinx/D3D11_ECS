#include  "Header\Structs.hlsli"
#include  "Header\Methods.hlsli"

//-----------------------------------------------------------------------------------
//! @brief      ���_�V�F�[�_�G���g���[�|�C���g
//-----------------------------------------------------------------------------------
PSInput VSFunc( VSInput input )
{
		PSInput output = ( PSInput ) 0;

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
void GSFunc( triangle  PSInput  input[3], inout  TriangleStream<PSInput>  stream )
{
		PSInput output;

		for ( int i = 0; i < 3; ++i )
		{
				output = ( PSInput ) 0;

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
PSOutput PSFunc( PSInput IN ) : SV_TARGET
{
		PSOutput  OUT = ( PSOutput ) 0;

		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );

		OUT.normal = OctEncode( IN.normal );
		OUT.color = meshColor * texel;
		OUT.specular = 0.2;

		return  OUT;
}