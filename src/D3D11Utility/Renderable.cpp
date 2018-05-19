//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <DirectXMath.h>
#include  <fbxsdk.h>


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


Renderable::Renderable()
{

}


Renderable::Renderable( PRIMITIVE_TYPE  primitiveType )
{
		m_isRendering = true;

		m_pVertexShader = new  VertexShader();
		m_pPixelShader = new  PixelShader();
		m_pGeometryShader = new  GeometryShader();

		VERTEX*  vertices;
		UINT  numVertices = CreatePrimitive( PRMTV_2D_SQUARE, Vector3( 0, 0, 0 ), Vector3( 1.0f, 1.0f, 1.0f ), vertices );
		m_pVertexBuffer = new  VertexBuffer( vertices, numVertices );

		INT  indices[ ] = { 0,1,2,0,2,3 };
		UINT  numIndex = ARRAYSIZE( indices );
		m_pVertexBuffer->CreateIndexBuffer( indices, numIndex );
		XMStoreFloat4x4( &m_cbuffer.world, XMMatrixTranslation( 0, 0, 0 ) );

		delete[ ]  vertices;
}


Renderable::Renderable( LPCSTR  fbxString )
{
		m_isRendering = true;

		m_pVertexShader = new  VertexShader();
		m_pPixelShader = new  PixelShader();
		m_pGeometryShader = new  GeometryShader();


		FbxManager*  fbxManager = FbxManager::Create();
		m_fbxScene = FbxScene::Create( fbxManager, "fbxscene" );
		FbxString  fileName( fbxString );
		FbxImporter*  fbxImporter = FbxImporter::Create( fbxManager, "imp" );
		fbxImporter->Initialize( fileName.Buffer(), -1, fbxManager->GetIOSettings() );
		fbxImporter->Import( m_fbxScene );

		FbxMesh*  mesh;
		INT  i = 0;
		INT  maxChildCounts = m_fbxScene->GetRootNode()->GetChildCount();

		for ( i = 0; i < maxChildCounts; i++ )
				if ( m_fbxScene->GetRootNode()->GetChild( i )->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh )
				{
						mesh = m_fbxScene->GetRootNode()->GetChild( i )->GetMesh();
						break;
				}

		INT  numVertices = mesh->GetControlPointsCount();
		VERTEX*  vertices = new  VERTEX[numVertices];
		for ( i = 0; i < ( int ) numVertices; i++ )
		{
				vertices[i].position.x = ( float ) mesh->GetControlPointAt( i )[0];
				vertices[i].position.y = ( float ) mesh->GetControlPointAt( i )[1];
				vertices[i].position.z = ( float ) mesh->GetControlPointAt( i )[2];
		}

		m_pVertexBuffer = new  VertexBuffer( vertices, ( UINT ) numVertices );
		m_pVertexBuffer->CreateIndexBuffer( mesh->GetPolygonVertices(), mesh->GetPolygonVertexCount() );

		m_pVertexBuffer->CreateRasterizer( D3D11_CULL_BACK, D3D11_FILL_SOLID );

		SafeDestroy( fbxImporter );
		//SafeDestroy( m_fbxScene );
		//SafeDestroy( fbxManager );
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