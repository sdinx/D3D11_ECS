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
#include  <D3D11Utility.h>
#include  <d3d11.h>
#include  <DirectXMath.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{
				class  VertexBuffer
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						VertexBuffer() = delete;
						VertexBuffer( VERTEX*  pVertices, UINT  numVertexCounts );
						VertexBuffer( PRIMITIVE_TYPE  primitiveType );
						~VertexBuffer();


				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						// プリミティブの種類
						D3D_PRIMITIVE_TOPOLOGY  primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
						// 頂点数
						UINT  m_numVertexCounts;
						// インデックスデータ数
						UINT  m_numIndexCounts;
						// 頂点データ
						VERTEX*  m_pVertices = nullptr;
						// 頂点バッファ
						ID3D11Buffer*  m_pVertexBuffer = nullptr;
						ID3D11Buffer*  m_pIndexBuffer = nullptr;

						ID3D11RasterizerState*  m_pRasterState = nullptr;
						UINT  m_nStride;
						UINT  m_nOffset = 0;


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

						// 頂点バッファの生成と設定
						HRESULT  CreateVertexBuffer();
						HRESULT  CreateIndexBuffer( const  INT*  nPrimitiveVertices, const  UINT  nIndexCounts );
						void  CreateRasterizer( D3D11_CULL_MODE  cullMode, D3D11_FILL_MODE  fillMode );
						// 描画を行うときに呼び出す
						void  BindBuffer();
						void  Release();

				}; // class VertexBuffer

		}// namespace Graphics
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_VERTEX_BUFFER_