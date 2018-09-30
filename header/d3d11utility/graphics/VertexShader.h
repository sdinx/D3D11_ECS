//----------------------------------------------------------------------------------
// file : VertexShader.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_GRAPHICS_VERTEX_SHADER_
#define  _INCLUDED_D3D11_UTILITY_GRAPHICS_VERTEX_SHADER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Graphics\IShader.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{

				class  VertexShader :public  IShader
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						VertexShader();
						VertexShader( ID3D11VertexShader*  pVS, ID3D11InputLayout*  pIL );
						~VertexShader();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------
						ID3D11VertexShader*  m_pVertexShader = nullptr;
						ID3D11InputLayout*  m_pInputLayout = nullptr;


				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------


				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						// �V�F�[�_�̃R���p�C���ƒ��_�V�F�[�_�̍쐬
						HRESULT  CreateVertexShader( LPCWSTR  szFileName = nullptr );
						// ���̓��C�A�E�g�̍쐬
						HRESULT  CreateInputLayout( ID3DBlob* pVSBlob );
						// �`����s���O�ɌĂяo��
						void  UpdateShader();
						void  Release();


				}; // class VertexShader

		} // namespace Graphics

} // namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GRAPHICS_VERTEX_SHADER_