//----------------------------------------------------------------------------------
// file: Texture.h
// desc: �e�N�X�`���̃T�C�Y��C���[�W�Ȃǂ̏����������N���X
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_TEXTURE_
#define  _INCLUDED_D3D11_UTILITY_TEXTURE_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility\Interface.h>


namespace  D3D11Utility
{
		const  int  TEXTURE_ID_INVALID = 0;

		namespace  Graphics
		{
				using  TextureId = int;

				class  Texture
				{
						friend  class  Systems::TextureManager;

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						Texture()
						{}
						Texture( ID3D11Texture2D*  texture, ID3D11ShaderResourceView*  shaderResourceView, ID3D11SamplerState*  sampler );
						virtual  ~Texture();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						ID3D11Texture2D*  m_texture = nullptr;
						ID3D11ShaderResourceView*  m_shaderResourceView = nullptr;
						ID3D11SamplerState*  m_sampler = nullptr;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						void  SetTexture();
						void  Release();

				};// class Texture

		}// namespace Graphics
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_TEXTURE_