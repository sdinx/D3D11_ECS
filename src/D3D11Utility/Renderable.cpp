//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <DirectXMath.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Graphics;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
INT  Renderable::STATIC_COMPONENT_ID = -1;
std::unique_ptr<CONSTANTBUFFER>  Renderable::s_pCBuffer = nullptr;


//----------------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------------
struct  ConstantBufferForPerFrame
{
		Matrix4x4  world;
};


Renderable::Renderable()
{}


Renderable::Renderable( PRIMITIVE_TYPE  primitiveType )
{
		m_isRendering = true;

		m_pVertexShader = std::unique_ptr<VertexShader>( new  VertexShader() );
		m_pPixelShader = std::unique_ptr<PixelShader>( new  PixelShader() );
		m_pGeometryShader = std::unique_ptr<GeometryShader>( new  GeometryShader() );

		VERTEX  vertices[ ] = {
				Vector3( 0.0f,0.5f,0.0f ),
				Vector3( 0.5f,-0.5f,0.0f ),
				Vector3( -0.5f,-0.5f,0.0f ),
		};
		UINT  numVertices = ARRAYSIZE( vertices );

		m_pVertexBuffer = std::unique_ptr<VertexBuffer>( new  VertexBuffer( vertices, numVertices ) );

		XMStoreFloat4x4( &m_localWorld, XMMatrixTranslation( 0, 0, 0 ) );
}


void  Renderable::SetConstantBuffer()
{
		if ( s_pCBuffer == nullptr )
		{
				s_pCBuffer = std::unique_ptr<CONSTANTBUFFER>( new  CONSTANTBUFFER );
				CreateConstantBuffer( &s_pCBuffer->pCB, s_pCBuffer->nCBSlot, sizeof( ConstantBufferForPerFrame ) );
		}
}


void  Renderable::Rendering()const
{
		m_pVertexShader->UpdateShader();
		m_pPixelShader->UpdateShader();
		m_pGeometryShader->UpdateShader();

		ConstantBufferForPerFrame  cbuffer;
		cbuffer.world = m_localWorld;
		pd3dDeviceContext->UpdateSubresource( s_pCBuffer->pCB, 0, nullptr, &cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( 1, 1, &s_pCBuffer->pCB );
		pd3dDeviceContext->PSSetConstantBuffers( 1, 1, &s_pCBuffer->pCB );
		pd3dDeviceContext->GSSetConstantBuffers( 1, 1, &s_pCBuffer->pCB );

		m_pVertexBuffer->BindBuffer();
}