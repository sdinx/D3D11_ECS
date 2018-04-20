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

				class  CVertexShader
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						CVertexShader();
						~CVertexShader();

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

						// 入力レイアウトの作成
						HRESULT  CreateInputLayout( ID3DBlob* pVSBlob );


				public:
						//----------------------------------------------------------------------------------
						// public functions
						//----------------------------------------------------------------------------------

						// シェーダのコンパイルと頂点シェーダの作成
						HRESULT  CreateVertexShader( LPCWSTR  szFileName = NULL );
						// 描画を行う前に呼び出す
						VOID  UpdateShader();
						VOID  Release();


				}; // class VertexShader

		} // namespace Graphics

} // namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_VERTEX_SHADER_