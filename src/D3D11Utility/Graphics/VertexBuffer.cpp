//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Graphics\VertexBuffer.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Graphics;


VertexBuffer::VertexBuffer( VERTEX*  pVertices, UINT  numVertexCounts )
{
		m_pVertices = new  VERTEX[numVertexCounts];
		for ( UINT i = 0; i < numVertexCounts; i++ )
				m_pVertices[i] = pVertices[i];
		m_numVertexCounts = numVertexCounts;

		CreateVertexBuffer();
}


VertexBuffer::VertexBuffer( PRIMITIVE_TYPE  primitiveType )
{
		m_pVertices = nullptr;
}


VertexBuffer::~VertexBuffer()
{
		Release();
}


HRESULT  VertexBuffer::CreateVertexBuffer()
{
		HRESULT  hr = S_OK;

		// 頂点バッファに頂点データを設定
		D3D11_BUFFER_DESC  bd;
		ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( VERTEX ) * m_numVertexCounts;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		// サブリソースの設定
		D3D11_SUBRESOURCE_DATA  initData;
		ZeroMemory( &initData, sizeof( D3D11_SUBRESOURCE_DATA ) );
		initData.pSysMem = m_pVertices;
		
		// 頂点バッファの生成
		hr = pd3dDevice->CreateBuffer( &bd, &initData, &m_pVertexBuffer );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<VertexBuffer> FAILED CreateBuffer (vertex buffer) \n" ) );
				return  hr;
		}

		m_nStride = sizeof( VERTEX );
		m_nOffset = 0;

		return  hr;
}


void  VertexBuffer::BindBuffer()
{
		// 入力アセンブラに頂点バッファを設定
		pd3dDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_nStride, &m_nOffset );

		// プリミティブの種類を設定
		pd3dDeviceContext->IASetPrimitiveTopology( primitiveType );

		pd3dDeviceContext->Draw( m_numVertexCounts, 0 );
}


void  VertexBuffer::Release()
{
		SafeRelease( m_pVertexBuffer );
		SafeDeleteArray( m_pVertices );
}