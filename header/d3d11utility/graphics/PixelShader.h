//----------------------------------------------------------------------------------
// file : PixelShader.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_GRAPHICS_PIXEL_SHADER_
#define  _INCLUDED_D3D11_UTILITY_GRAPHICS_PIXEL_SHADER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Graphics\IShader.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{
				class  PixelShader :public  IShader
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						PixelShader();
						PixelShader( ID3D11PixelShader*  pPS );
						~PixelShader();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------
						ID3D11PixelShader*  m_pPixelShader;
						ID3D11SamplerState*  m_pSamplerState;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------


				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------


				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						// �V�F�[�_�̃R���p�C���ƒ��_�V�F�[�_�̍쐬
						HRESULT  CreatePixelShader( LPCWSTR  szFileName = nullptr );
						HRESULT  CreateSamplerState();
						// �`����s���O�ɌĂяo��
						void  UpdateShader();
						void  Release();

				}; // class PixelBuffer

		}// namespace Graphics

}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GRAPHICS_PIXEL_SHADER_