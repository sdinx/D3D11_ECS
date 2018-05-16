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
using  namespace  GameUtility;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentId  Renderable::STATIC_COMPONENT_ID = STATIC_ID_INVALID;
std::unique_ptr<CONSTANTBUFFER>  Renderable::s_pCBuffer = nullptr;


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

		XMStoreFloat4x4( &m_cbuffer.world, XMMatrixTranslation( 0, 0, 0 ) );
}


void  Renderable::SetConstantBuffer()
{
		if ( s_pCBuffer == nullptr )
		{
				s_pCBuffer = std::unique_ptr<CONSTANTBUFFER>( new  CONSTANTBUFFER );
				CreateConstantBuffer( &s_pCBuffer->pCB, s_pCBuffer->nCBSlot, sizeof( ConstantBufferForPerFrame ) );
		}
}


void  Renderable::HandleMessage( const  Message&  msg )
{
		switch ( msg.messageType )
		{
		case  MSG_UPDATE_CBUFFER:
				{
						Transform*  transform = GetComponent<Transform>();
						if ( transform == nullptr )
								return;
						UpdateConstantBuffer( transform->GetLocalWorld() );
				}// end case MSG_UPDATE_CBUFFER
				break;
		default: {return; }
		}// end switch

}// end HandleMessage(const Message&) : void


void  Renderable::Rendering()const
{
		if ( m_isActive == false )
				return;

		m_pVertexShader->UpdateShader();
		m_pPixelShader->UpdateShader();
		m_pGeometryShader->UpdateShader();

		pd3dDeviceContext->UpdateSubresource( s_pCBuffer->pCB, 0, nullptr, &m_cbuffer, 0, 0 );
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
		transform->Update();
		Vector3&  pos = transform->GetPosition();

		pos.x = 100;
}


void  Renderable::UpdateConstantBuffer( Matrix4x4  world )
{
		m_cbuffer.world = world;
}