//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\TextureManager.h>
#include  <DirectXMath.h>
#include  <D3D11Utility\Systems\FbxLoader.h>


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
		m_pVertexShader( nullptr ),
		m_pGeometryShader( nullptr ),
		m_pPixelShader( nullptr )
{

}


Renderable::Renderable( PRIMITIVE_TYPE  primitiveType ) :
		m_pVertexShader( nullptr ),
		m_pGeometryShader( nullptr ),
		m_pPixelShader( nullptr )
{
		m_isRendering = true;

		VERTEX*  vertices;
		INT* indices;
		UINT  numVertices = CreatePrimitive( PRMTV_CUBE, Vector3( 0, 0, 0 ), Vector3( 1.0f, 1.0f, 1.0f ), vertices, indices );
		m_pVertexBuffer = new  VertexBuffer( vertices, numVertices );

		UINT  numIndex = numVertices * 6;
		m_pVertexBuffer->CreateIndexBuffer( indices, numIndex );
		XMStoreFloat4x4( &m_cbuffer.world, XMMatrixTranslation( 0, 0, 0 ) );
		m_pVertexBuffer->CreateRasterizer( D3D11_CULL_NONE, D3D11_FILL_SOLID );

		delete[ ]  vertices;
}


Renderable::Renderable( LPCSTR  fbxString ) :
		m_pVertexShader( nullptr ),
		m_pGeometryShader( nullptr ),
		m_pPixelShader( nullptr )
{
		m_isRendering = true;

		Systems::FbxLoader  loader( fbxString );

		ModelContainer  container = loader.GetModelContainer( 0 );

		INT  i = 0;

		INT  vertexCount = container.vertices.size();
		VERTEX*  vertices = new  VERTEX[vertexCount];
		for ( auto vertex : container.vertices )
		{
				vertices[i].position = vertex;
				i++;
		}

		i = 0;
		for ( auto texcoord : container.texcoords )
		{
				vertices[i].texcoord = texcoord;
				i++;
		}

		INT  indexCount = container.indices.size();
		INT*  indices = new  INT[indexCount];
		i = 0;
		for ( auto index : container.indices )
		{
				indices[i] = index;
				i++;
		}

		m_pVertexBuffer = new  VertexBuffer( vertices, ( UINT ) vertexCount );

		// note: インデックスが正しく設定されていない?
		//m_pVertexBuffer->CreateIndexBuffer( indices, indexCount );


		m_pVertexBuffer->CreateRasterizer( D3D11_CULL_NONE, D3D11_FILL_SOLID );


		delete[ ]  vertices;
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
						transform->Update();
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

		if ( m_textureId != TEXTURE_ID_INVALID )
				m_textureManager->SetTexture( m_textureId );
		else
		{
				// テクスチャのNULL化
		}// end else

		m_pVertexShader->UpdateShader();
		m_pPixelShader->UpdateShader();
		//m_pGeometryShader->UpdateShader();

		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &m_cbuffer, 0, 0 );

		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->PSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		//pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );

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

		HandleMessage( MSG_UPDATE_CBUFFER );
}


void  Renderable::UpdateConstantBuffer( Matrix4x4  world )
{
		m_cbuffer.world = world;
}


void  Renderable::SetColor( Vector4  v4Color )
{
		m_cbuffer.meshColor = v4Color;
}


void  Renderable::SetTextureId( Graphics::TextureId  textureId, Systems::TextureManager*  textureManagerInstance )
{
		if ( textureManagerInstance != nullptr )
				m_textureManager = textureManagerInstance;


		m_textureId = textureId;
}