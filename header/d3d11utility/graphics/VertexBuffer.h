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
#include  <d3d11utility/Interface.h>
#include  <d3d11.h>
#include  <DirectXMath.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{
				using  MeshId = int;

				class  VertexBuffer :public  Object
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						VertexBuffer() = delete;
						VertexBuffer( VERTEX*  pVertices, uint  nVertexCounts, const  MeshId  meshId, const  Systems::FbxLoader&  fbxLoader );
						VertexBuffer( void*  pData, uint  nVertexCounts, uint  nStride, const  MeshId  meshId, const  Systems::FbxLoader&  fbxLoader );
						~VertexBuffer();


				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						// プリミティブの種類
						D3D_PRIMITIVE_TOPOLOGY  primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
						// 頂点数
						uint  m_nVertexCounts;
						// インデックスデータ数
						uint  m_nIndexCounts;
						// 頂点データ
						VERTEX*  m_pVertices = nullptr;
						// 頂点バッファ
						ID3D11Buffer*  m_pVertexBuffer = nullptr;
						ID3D11Buffer*  m_pIndexBuffer = nullptr;

						const  Systems::FbxLoader*  m_fbxLoader;
						const  MeshId  m_meshId;
						const  uint  m_nStride;
						uint  m_nOffset = 0;


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
						HRESULT  CreateVertexBuffer( void*  pData, uint  nVertexCounts, uint  nStride, uint  nOffset = 0 );
						HRESULT  CreateIndexBuffer( const  int*  nPrimitiveVertices, const  uint  nIndexCounts );
						// Getter
						uint  GetVertexCounts() { return  m_nVertexCounts; }
						MeshId  GetMeshId()
						{
								return  m_meshId;
						}
						const  Systems::FbxLoader*  GetFbxLoader()
						{
								return  m_fbxLoader;
						}
						void  GetVertex( VERTEX*&  pData, uint&  nVertexCounts )
						{
								pData = m_pVertices;
								nVertexCounts = m_nVertexCounts;
						}
						// 描画を行うときに呼び出す
						void  BindBuffer();
						void  Render();
						void  Release();

				}; // class VertexBuffer

		}// namespace Graphics
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_VERTEX_BUFFER_