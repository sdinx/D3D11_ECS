//----------------------------------------------------------------------------------
// file : VertexShader.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_VERTEX_SHADER_
#define  _INCLUDED_D3D11_UTILITY_VERTEX_SHADER_

//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
#pragma  comment(lib,  "d3d11.lib")

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{

				class  VertexShader
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						VertexShader();
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
						// private functions
						//----------------------------------------------------------------------------------

						// ���̓��C�A�E�g�̍쐬
						HRESULT  CreateInputLayout( ID3DBlob* pVSBlob );


				public:
						//----------------------------------------------------------------------------------
						// public functions
						//----------------------------------------------------------------------------------

						// �V�F�[�_�̃R���p�C���ƒ��_�V�F�[�_�̍쐬
						HRESULT  CreateVertexShader( LPCWSTR  szFileName = nullptr );
						// �`����s���O�ɌĂяo��
						void  UpdateShader();
						void  Release();


				}; // class VertexShader

		} // namespace Graphics

} // namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_VERTEX_SHADER_