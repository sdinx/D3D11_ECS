//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Graphics\VertexBuffer.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Graphics;


VertexBuffer::VertexBuffer( VERTEX*  pVertices, uint  nVertexCounts, const  MeshId  meshId, const  Systems::FbxLoader&  fbxLoader ) :
		m_nStride( sizeof( VERTEX ) ),
		m_meshId( meshId ),
		m_fbxLoader( &fbxLoader )
{
		m_pVertices = new  VERTEX[nVertexCounts];
		for ( uint i = 0; i < nVertexCounts; i++ )
				m_pVertices[i] = pVertices[i];

		CreateVertexBuffer( pVertices, nVertexCounts, sizeof( VERTEX ) );
}


VertexBuffer::VertexBuffer( void*  pData, uint  nVertexCounts, uint  nStride, const  MeshId  meshId, const  Systems::FbxLoader&  fbxLoader ) :
		m_nStride( nStride ),
		m_meshId( meshId ),
		m_fbxLoader( &fbxLoader )
{


		CreateVertexBuffer( pData, nVertexCounts, nStride );
}


VertexBuffer::~VertexBuffer()
{
		Release();
}


HRESULT  VertexBuffer::CreateVertexBuffer( void*  pData, uint  nVertexCounts, uint  nStride, uint  nOffset )
{
		HRESULT  hr = S_OK;

		// 頂点バッファに頂点データを設定
		D3D11_BUFFER_DESC  bd;
		ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = nStride * nVertexCounts;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA  initData;
		ZeroMemory( &initData, sizeof( D3D11_SUBRESOURCE_DATA ) );
		initData.pSysMem = pData;

		// 頂点バッファの生成
		hr = pd3dDevice->CreateBuffer( &bd, &initData, &m_pVertexBuffer );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<VertexBuffer> FAILED CreateBuffer (vertex buffer) \n" ) );
				return  hr;
		}

		m_nVertexCounts = nVertexCounts;
		m_nOffset = nOffset;

		return  hr;
}


HRESULT  VertexBuffer::CreateIndexBuffer( const  int*  nPrimitiveVertices, const  uint  nIndexCounts )
{
		HRESULT  hr = S_OK;
		m_nIndexCounts = nIndexCounts;

		// インデックスバッファに頂点データを設定
		D3D11_BUFFER_DESC  bd;
		ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( INT ) * nIndexCounts;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA  initData;
		ZeroMemory( &initData, sizeof( D3D11_SUBRESOURCE_DATA ) );
		initData.pSysMem = nPrimitiveVertices;

		// 頂点バッファの生成
		hr = pd3dDevice->CreateBuffer( &bd, &initData, &m_pIndexBuffer );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<VertexBuffer> FAILED CreateBuffer (vertex buffer) \n" ) );
				return  hr;
		}

		return  hr;
}


void  VertexBuffer::BindBuffer()
{
		// 入力アセンブラに頂点バッファを設定
		pd3dDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_nStride, &m_nOffset );
		pd3dDeviceContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

		// プリミティブの種類を設定
		pd3dDeviceContext->IASetPrimitiveTopology( primitiveType );

		//if ( m_pIndexBuffer == nullptr )
		pd3dDeviceContext->Draw( m_nVertexCounts, 0 );
		//else
		//		pd3dDeviceContext->DrawIndexed( m_numIndexCounts, 0, 0 );
}


void  VertexBuffer::Release()
{
		SafeRelease( m_pVertexBuffer );
		SafeDeleteArray( m_pVertices );
}