//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <VertexBuffer.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Graphics;


CVertexBuffer::CVertexBuffer( VERTEX*  pVertices, UINT  numVertexCounts )
{
		m_pVertices = new VERTEX( *pVertices );
		m_numVertexCounts = numVertexCounts;

		CreateVertexBuffer();
}


CVertexBuffer::CVertexBuffer( POLYGON_TYPE  polygonType )
{
		m_pVertices = NULL;
}


CVertexBuffer::~CVertexBuffer()
{
		Release();
}


HRESULT  CVertexBuffer::CreateVertexBuffer()
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
				OutputDebugString( TEXT( "<CVertexBuffer> FAILED CreateBuffer (vertex buffer) \n" ) );
				return  hr;
		}


		m_nStride = sizeof( VERTEX );
		m_nOffset = 0;


		return  hr;
}


VOID  CVertexBuffer::BindBuffer()
{
		// 入力アセンブラに頂点バッファを設定
		pd3dDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_nStride, &m_nOffset );

		// プリミティブの種類を設定
		pd3dDeviceContext->IASetPrimitiveTopology( primitiveType );

		pd3dDeviceContext->Draw( m_numVertexCounts, 0 );
}


VOID  CVertexBuffer::Release()
{
		SAFE_RELEASE( m_pVertexBuffer );
		SAFE_DELETEARRAY( m_pVertices );
}