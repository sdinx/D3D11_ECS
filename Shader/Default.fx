#include  "Header\Structs.hlsli"
#include  "Header\Methods.hlsli"

//-----------------------------------------------------------------------------------
//! @brief      ���_�V�F�[�_�G���g���[�|�C���g
//-----------------------------------------------------------------------------------
PSInput VSFunc( VSInput IN )
{
		PSInput OUT = ( PSInput ) 0;

		// ���[���h��Ԃɕϊ�
		OUT.position = mul( g_world, IN.position );

		// �r���[��Ԃɕϊ�
		OUT.position = mul( g_view, OUT.position );

		// �ˉe��Ԃɕϊ�
		OUT.position = mul( g_proj, OUT.position );

		OUT.normal = normalize( mul( ( float3x3 )g_world, IN.normal ) );
		OUT.texcoord = IN.texcoord;

		return OUT;
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
				output.position = mul( g_world, input[i].position );

				// �r���[��Ԃɕϊ�
				output.position = mul( g_view, output.position );

				// �ˉe��Ԃɕϊ�
				output.position = mul( g_proj, output.position );

				// �X�g���[���ɒǉ�
				stream.Append( output );
		}
		stream.RestartStrip();
}

//------------------------------------------------------------------------------------
//! @brief      �s�N�Z���V�F�[�_�G���g���[�|�C���g
//------------------------------------------------------------------------------------
PSOutput PSFunc( PSInput IN )// : SV_Target
{
		PSOutput  OUT = ( PSOutput ) 0;

		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, IN.texcoord );

		OUT.normal = OctEncode( IN.normal );
		OUT.color = g_diffuse * texel;
		OUT.specular = g_specular.w;

		return  OUT;
}