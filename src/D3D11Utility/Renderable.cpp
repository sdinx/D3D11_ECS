//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
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
ComponentId  Renderable::STATIC_COMPONENT_ID = STATIC_ID_INVALID;
std::unique_ptr<CONSTANTBUFFER>  Renderable::s_pCBuffer = nullptr;


//----------------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------------
struct  ConstantBufferForPerFrame
{
		Matrix4x4  world;
		Vector4  meshColor;
};


Renderable::Renderable()
{

}


Renderable::Renderable( PRIMITIVE_TYPE  primitiveType )
{
		m_isRendering = true;

		m_pVertexShader = new  VertexShader();
		m_pPixelShader = new  PixelShader();
		m_pGeometryShader = new  GeometryShader();

		VERTEX  vertices[ ] = {
				Vector3( 0.0f,0.5f,0.0f ),
				Vector3( 0.5f,-0.5f,0.0f ),
				Vector3( -0.5f,-0.5f,0.0f ),
		};
		UINT  numVertices = ARRAYSIZE( vertices );

		m_pVertexBuffer = new  VertexBuffer( vertices, numVertices );

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
		if ( m_isActive == false )
				return;

		m_pVertexShader->UpdateShader();
		m_pPixelShader->UpdateShader();
		m_pGeometryShader->UpdateShader();

		ConstantBufferForPerFrame  cbuffer;
		cbuffer.world = m_localWorld;
		cbuffer.meshColor = m_meshColor;

		pd3dDeviceContext->UpdateSubresource( s_pCBuffer->pCB, 0, nullptr, &cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( 1, 1, &s_pCBuffer->pCB );
		pd3dDeviceContext->PSSetConstantBuffers( 1, 1, &s_pCBuffer->pCB );
		pd3dDeviceContext->GSSetConstantBuffers( 1, 1, &s_pCBuffer->pCB );

		m_pVertexBuffer->BindBuffer();
}


void  Renderable::Update()
{
		if ( m_isUpdating == false )
				return;


		Transform*  transform = m_pComponentManager->GetComponent<Transform>( m_parentsEntityId );
		if ( transform == nullptr )
				return;

		Vector3&  pos = transform->GetPosition();

		pos.x = 100;
}


void  Renderable::SetColor( Vector4  v4Color )
{
		m_meshColor = v4Color;
}