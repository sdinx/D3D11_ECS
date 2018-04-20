//----------------------------------------------------------------------------------
// file : VertexBuffer.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_VERTEX_BUFFER_
#define  _INCLUDED_D3D11_UTILITY_VERTEX_BUFFER_

//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
#pragma comment(lib, "d3d11.lib")

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11.h>
#include  <DirectXMath.h>
#include  <D3D11Utility.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{
				class  CVertexBuffer
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						CVertexBuffer() = delete;
						CVertexBuffer( VERTEX*  pVertices, UINT  numVertexCounts );
						CVertexBuffer( POLYGON_TYPE  polygonType );
						~CVertexBuffer();


				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						// プリミティブの種類
						D3D_PRIMITIVE_TOPOLOGY  primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
						// 頂点数
						UINT  m_numVertexCounts;
						// 頂点データ
						VERTEX  *m_pVertices = NULL;
						// 頂点バッファ
						ID3D11Buffer*  m_pVertexBuffer = NULL;
						UINT  m_nStride;
						UINT  m_nOffset = 0;


				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------


				private:
						//----------------------------------------------------------------------------------
						// private functions
						//----------------------------------------------------------------------------------


				public:
						//----------------------------------------------------------------------------------
						// public functions
						//----------------------------------------------------------------------------------

						// 頂点バッファの生成と設定
						HRESULT  CreateVertexBuffer();
						// 描画を行うときに呼び出す
						VOID  BindBuffer();
						VOID  Release();

				}; // class VertexBuffer

		}// namespace Graphics
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_VERTEX_BUFFER_