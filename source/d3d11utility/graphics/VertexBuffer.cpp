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

		// ���_�o�b�t�@�ɒ��_�f�[�^��ݒ�
		D3D11_BUFFER_DESC  bd;
		ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( VERTEX ) * m_numVertexCounts;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA  initData;
		ZeroMemory( &initData, sizeof( D3D11_SUBRESOURCE_DATA ) );
		initData.pSysMem = m_pVertices;

		// ���_�o�b�t�@�̐���
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


HRESULT  VertexBuffer::CreateIndexBuffer( const  INT*  nPrimitiveVertices, const  UINT  nIndexCounts )
{
		HRESULT  hr = S_OK;
		m_numIndexCounts = nIndexCounts;

		// �C���f�b�N�X�o�b�t�@�ɒ��_�f�[�^��ݒ�
		D3D11_BUFFER_DESC  bd;
		ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( INT ) * nIndexCounts;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		// �T�u���\�[�X�̐ݒ�
		D3D11_SUBRESOURCE_DATA  initData;
		ZeroMemory( &initData, sizeof( D3D11_SUBRESOURCE_DATA ) );
		initData.pSysMem = nPrimitiveVertices;

		// ���_�o�b�t�@�̐���
		hr = pd3dDevice->CreateBuffer( &bd, &initData, &m_pIndexBuffer );
		if ( FAILED( hr ) )
		{
				OutputDebugString( TEXT( "<VertexBuffer> FAILED CreateBuffer (vertex buffer) \n" ) );
				return  hr;
		}

		return  hr;
}


void  VertexBuffer::CreateRasterizer( D3D11_CULL_MODE  cullMode, D3D11_FILL_MODE  fillMode )
{
		D3D11_RASTERIZER_DESC  rdc = {};
		rdc.CullMode = cullMode;
		rdc.FillMode = fillMode;
		rdc.FrontCounterClockwise = TRUE;
		pd3dDevice->CreateRasterizerState( &rdc, &m_pRasterState );
}


void  VertexBuffer::BindBuffer()
{
		// ���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
		pd3dDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &m_nStride, &m_nOffset );
		pd3dDeviceContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

		// ���X�^���C�Y
		pd3dDeviceContext->RSSetState( m_pRasterState );
		// �v���~�e�B�u�̎�ނ�ݒ�
		pd3dDeviceContext->IASetPrimitiveTopology( primitiveType );

		if ( m_pIndexBuffer == nullptr )
				pd3dDeviceContext->Draw( m_numVertexCounts, 0 );
		else
				pd3dDeviceContext->DrawIndexed( m_numIndexCounts, 0, 0 );
}


void  VertexBuffer::Release()
{
		SafeRelease( m_pVertexBuffer );
		SafeDeleteArray( m_pVertices );
}