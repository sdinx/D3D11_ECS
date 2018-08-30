#include  "Header\Structs.hlsli"

float3  NormalizedLambert( float3  diffuse, float3  lightDir, float3  normal )
{
		return diffuse * max( dot( normal, lightDir ), 0.0f ) * ( 1.0f / PI );
}

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
		float3  light = normalize( float4( 2, 5, -3, 1.0f ) );

		float4  texel = diffuseTexture.Sample( diffuseTextureSampler, input.texcoord );
		
		float4  color = meshColor * texel;
		
		float3  nor = normalize( input.normal );
		float3  diffuse = NormalizedLambert( float4( 0.5f, 0.5f, 0.5f, 1.0f )*texel.rgb, light, nor );

		//return  color;
		return  float4( diffuse, 1.0f );
}