//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/Renderable.h>
#include  <d3d11utility\components/Transform.h>
#include  <d3d11utility\graphics\Material.h>
#include  <d3d11utility\Systems\ComponentManager.h>
#include  <d3d11utility\Systems\IDirect3DRenderer.h>
#include  <d3d11utility\Systems\TextureManager.h>
#include  <DirectXMath.h>
#include  <omp.h>


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
const  UINT  Renderable::s_nConstantBufferSlot;
ID3D11Buffer*  Renderable::s_pConstantBuffer = nullptr;


Renderable::Renderable() :
		m_pVertexBuffer( nullptr ),
		m_pVertexShader( nullptr ),
		m_pGeometryShader( nullptr ),
		m_pPixelShader( nullptr ),
		m_meshId( 0 ),
		m_materialId( 1 ),
		m_diffuseId( TEXTURE_ID_INVALID ),
		m_normalId( TEXTURE_ID_INVALID )
{

}


Renderable::Renderable( Graphics::MeshId  meshId, Graphics::eRasterMode  rasterMode, Graphics::MaterialId  id ) :
		m_pVertexBuffer( nullptr ),
		m_pVertexShader( nullptr ),
		m_pGeometryShader( nullptr ),
		m_pPixelShader( nullptr ),
		m_meshId( meshId ),
		m_materialId( id ),
		m_diffuseId( TEXTURE_ID_INVALID ),
		m_normalId( TEXTURE_ID_INVALID ),
		m_nRasterMode( rasterMode )
{
		m_isRendering = true;
		m_pRenderer = _Singleton<Systems::IDirect3DRenderer>::GetInstance();

		m_pVertexBuffer = m_pRenderer->GetVertexBuffer( meshId );

		auto  fbxLoader = m_pVertexBuffer->GetFbxLoader();
		Material  material = fbxLoader->GetMaterial( 0 );
}


Renderable::Renderable( Graphics::VertexBuffer*  pVertexBuffer, Graphics::eRasterMode  rasterMode, Graphics::MaterialId  id ) :
		m_pVertexBuffer( pVertexBuffer ),
		m_pVertexShader( nullptr ),
		m_pGeometryShader( nullptr ),
		m_pPixelShader( nullptr ),
		m_meshId( 0 ),
		m_materialId( id ),
		m_diffuseId( TEXTURE_ID_INVALID ),
		m_normalId( TEXTURE_ID_INVALID ),
		m_nRasterMode( rasterMode )
{
		m_isRendering = true;
		m_pRenderer = _Singleton<Systems::IDirect3DRenderer>::GetInstance();

		m_pVertexBuffer = m_pRenderer->GetVertexBuffer( pVertexBuffer->GetMeshId() );

		auto  fbxLoader = m_pVertexBuffer->GetFbxLoader();
		Material  material = fbxLoader->GetMaterial( 0 );
}


Renderable::~Renderable()
{
		Release();
}


void  Renderable::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( ConstantBufferForPerFrame ) );
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
						UpdateConstantBuffer( transform->GetWorld() );
				}// end case MSG_UPDATE_CBUFFER
				break;
		default: {return; }
		}// end switch

}// end HandleMessage(const Message&) : void


void  Renderable::Rendering()const
{
		if ( m_isActive == false )
				return;

		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &m_cbuffer, 0, 0 );

		m_pVertexShader->UpdateShader();
		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );

		m_pPixelShader->UpdateShader();
		pd3dDeviceContext->PSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );

		/*
				m_pGeometryShader->UpdateShader();
				pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		*/

		m_pVertexBuffer->BindBuffer();

}


void  Renderable::Update()
{
		//if ( m_isUpdating == false )
		//		return;
		//
		//Transform*  transform = m_pComponentManager->GetComponent<Transform>( m_parentsEntityId );
		//if ( transform == nullptr )
		//		return;
		//
		//HandleMessage( MSG_UPDATE_CBUFFER );
}


void  Renderable::UpdateConstantBuffer( Matrix4x4  world )
{
		m_cbuffer.world = world;
}


Graphics::Material*  Renderable::GetMaterial()
{
		return  m_pRenderer->GetMaterial( m_materialId );
}


void  Renderable::SetMaterial( Graphics::Material*  material )
{
		m_materialId = material->GetMaterialId();
}


void  Renderable::SetMaterial( Graphics::MaterialId  id )
{
		m_materialId = id;
}


void  Renderable::SetDiffuseTexId( Graphics::TextureId  textureId )
{
		m_diffuseId = textureId;
}


void  Renderable::SetNormalTexId( Graphics::TextureId  textureId )
{
		m_normalId = textureId;
}


void  Renderable::Release()
{
		//m_pRenderer.~shared_ptr();
}