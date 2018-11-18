//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/Renderable.h>
#include  <d3d11utility\components/Transform.h>
#include  <d3d11utility\Systems\ComponentManager.h>
#include  <d3d11utility\Systems\TextureManager.h>
#include  <d3d11utility\Systems\FbxLoader.h>
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
		m_textureId( TEXTURE_ID_INVALID )
{

}


Renderable::Renderable( ePrimitiveType  primitiveType, D3D11_CULL_MODE  cullMode, D3D11_FILL_MODE  fillMode ) :
		m_pVertexBuffer( nullptr ),
		m_pVertexShader( nullptr ),
		m_pGeometryShader( nullptr ),
		m_pPixelShader( nullptr ),
		m_textureId( TEXTURE_ID_INVALID )
{
		m_isRendering = true;

		/* 頂点の設定 */
		VERTEX*  vertices;
		INT* indices;
		// 定数指定でプリミティブの頂点を生成
		UINT  numVertices = CreatePrimitive( primitiveType, Vector3( 0, 0, 0 ), Vector3( 1.0f, 1.0f, 1.0f ), vertices, indices );
		m_pVertexBuffer = new  VertexBuffer( vertices, numVertices );

		/* インデックス頂点の設定 */
		UINT  numIndex = numVertices * 6;
		m_pVertexBuffer->CreateIndexBuffer( indices, numIndex );

		XMStoreFloat4x4( &m_cbuffer.world, XMMatrixTranslation( 0, 0, 0 ) );
		// カリング設定
		m_pVertexBuffer->CreateRasterizer( cullMode, fillMode );

		delete[ ]  vertices;
}


Renderable::Renderable( LPCSTR  fbxString, D3D11_CULL_MODE  cullMode, D3D11_FILL_MODE  fillMode ) :
		m_pVertexBuffer( nullptr ),
		m_pVertexShader( nullptr ),
		m_pGeometryShader( nullptr ),
		m_pPixelShader( nullptr ),
		m_textureId( TEXTURE_ID_INVALID )
{
		m_isRendering = true;

		// 指定ディレクトリのFBXモデルをロード
		Systems::FbxLoader  loader( fbxString );
		// モデルの情報を取得 ( 0番目のメッシュ情報 )
		ModelContainer  container = loader.GetModelContainer( 0 );

		INT  i = 0;

		INT  vertexCount = container.vertices.size();
		VERTEX*  vertices = new  VERTEX[vertexCount];
		for ( auto vertex : container.vertices )
		{
				vertices[i].position = DirectX::XMFLOAT3( vertex.m_floats[0], vertex.m_floats[1], vertex.m_floats[2] );;
				i++;
		}

		i = 0;
		for ( auto texcoord : container.texcoords )
		{
				vertices[i].texcoord = texcoord;
				i++;
		}

		i = 0;
		for ( auto normal : container.normals )
		{
				vertices[i].normal = DirectX::XMFLOAT3( normal.m_floats[0], normal.m_floats[1], normal.m_floats[2] );
				i++;
		}

		int ad = ( int ) vertices[4].normal.x;
		m_pVertexBuffer = new  VertexBuffer( vertices, ( UINT ) vertexCount );

		SetDiffuse( loader.GetMaterial( 0 ).diffuse );

		// note: インデックスが正しく設定されていない?
		//m_pVertexBuffer->CreateIndexBuffer( container.indices.data(), container.indices.size() );

		m_pVertexBuffer->CreateRasterizer( D3D11_CULL_NONE, fillMode );
		//m_pVertexBuffer->CreateRasterizer( D3D11_CULL_NONE, fillMode );


		delete[ ]  vertices;
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

void  Renderable::SetAmbient( Vector4  v4Color )
{
		m_cbuffer.ambient = v4Color;
}


void  Renderable::SetDiffuse( Vector4  v4Color )
{
		m_cbuffer.diffuse = v4Color;
}


void  Renderable::SetEmissive( Vector4  v4Color )
{
		m_cbuffer.emissive = v4Color;
}


void  Renderable::SetSpecular( Vector4  v4Color )
{
		m_cbuffer.specular = v4Color;
}


void  Renderable::SetTextureId( Graphics::TextureId  textureId )
{
		m_textureId = textureId;
}


void  Renderable::Release()
{
		SafeRelease( m_pVertexBuffer );
}