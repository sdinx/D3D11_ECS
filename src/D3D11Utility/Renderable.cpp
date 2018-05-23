//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\TextureManager.h>
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
		m_pVertexBuffer->CreateRasterizer( D3D11_CULL_NONE, D3D11_FILL_SOLID );

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
				}// end if

		INT  numVertices = mesh->GetControlPointsCount();
		VERTEX*  vertices = new  VERTEX[numVertices];
		INT*  index = mesh->GetPolygonVertices();
		FbxVector4  vec;
		for ( i = 0; i < numVertices; i++ )
		{
				vec = mesh->GetControlPointAt( i );
				vertices[i].position.x = ( float )vec[0];
				vertices[i].position.y = ( float )vec[1];
				vertices[i].position.z = ( float )vec[2];
		}// end for

		FbxTextureInfo  texInfo = FbxLoadTexcoord( mesh );
		
		i = 0;
		for ( auto texcoord : texInfo.texcoordList )
		{
				vertices[index[i]].texcoord = texcoord;
				i++;
		}

		m_pVertexBuffer = new  VertexBuffer( vertices, ( UINT ) numVertices );
		m_pVertexBuffer->CreateIndexBuffer( mesh->GetPolygonVertices(), mesh->GetPolygonVertexCount() );


		m_pVertexBuffer->CreateRasterizer( D3D11_CULL_BACK, D3D11_FILL_SOLID );

		SafeDestroy( fbxImporter );
		SafeDestroy( m_fbxScene );
		SafeDestroy( fbxManager );
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


FbxTextureInfo  Renderable::FbxLoadTexcoord( FbxMesh*  fbxMesh )
{
		FbxTextureInfo  fbxTextureInfo;
		FbxVector2  uv;
		FbxGeometryElementUV*  UV = nullptr;
		FbxGeometryElement::EMappingMode  mapping;
		FbxGeometryElement::EReferenceMode  reference;
		FbxLayerElementArrayTemplate<int>*  indexArray;
		const FbxLayerElementArrayTemplate<FbxVector2>*  directArray;
		int  size = fbxMesh->GetPolygonCount();
		INT*  indexList = fbxMesh->GetPolygonVertices();
		int  uvCount = 0;
		int  k = 0;
		int  j = 0;
		int  index = 0;
		int  uvIndex = 0;
		int  polygonSize = 0;
		int  indexByPolygonVertex = 0;						// 面の構成情報インデックス配列のインデックス
		int  polygonCount = 0;

		// UVセット数を取得
		int UVLayerCount = fbxMesh->GetElementUVCount();
		for ( int i = 0; UVLayerCount > i; i++ ) 
		{
				// UVバッファを取得
				UV = fbxMesh->GetElementUV( i );
				indexArray = &UV->GetIndexArray();
				directArray = &UV->GetDirectArray();

				// マッピングモードの取得
				mapping = UV->GetMappingMode();
				// リファレンスモードの取得
				reference = UV->GetReferenceMode();

				// UV数を取得
				uvCount = UV->GetDirectArray().GetCount();

				// 頂点に対応して格納されている場合
				if ( mapping == FbxGeometryElement::eByControlPoint )
				{
						// 頂点座標でマッピング
						for ( j = 0; j < size; j++ )
						{
								index = indexList[j];			// 面の構成情報配列から頂点インデックス番号を取得

																							// リファレンスモードを判定
								uvIndex;
								if ( reference == FbxGeometryElement::eDirect ) {		// eDirectの場合
										uvIndex = index;		//　eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
								}
								else {													// eIndexToDirectの場合
										uvIndex = indexArray->GetAt( index );				// 頂点座標インデックスに対応したＵＶ情報インデックスを取得
								}

								FbxVector2 uv = directArray->GetAt( uvIndex );		// uv値をdouble型で取得
								fbxTextureInfo.texcoordList.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );		// float値として格納
						}
				}
				// 面の構成情報に対応して格納されている場合
				else if ( mapping == FbxGeometryElement::eByPolygonVertex )
				{
						// ポリゴンバーテックス（面の構成情報のインデックス）でマッピング
						indexByPolygonVertex = 0;
						polygonCount = fbxMesh->GetPolygonCount();			// メッシュのポリゴン数を取得
						for ( j = 0; j < polygonCount; ++j )				// ポリゴン数分ループ
						{
								polygonSize = fbxMesh->GetPolygonSize( j );		// ｉ番目のポリゴン頂点数を取得

																																// ポリゴンの頂点数分ループ
								for ( k = 0; k < polygonSize; ++k )
								{
										// リファレンスモードの判定？
										if ( reference == FbxGeometryElement::eDirect ) {		// eDirectの場合
												uvIndex = indexByPolygonVertex;		//　eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
										}
										else {													// eIndexToDirectの場合
												uvIndex = indexArray->GetAt( indexByPolygonVertex );	// 面の構成情報インデックスに対応したＵＶ情報インデックスを取得
										}
										uv = directArray->GetAt( uvIndex );

										fbxTextureInfo.texcoordList.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );	// ｆｌｏａｔ値として格納

										++indexByPolygonVertex;						// 頂点インデックスをインクリメント
								}
						}
				}
				else
				{
						// それ以外のマッピングモードには対応しない
						assert( false );
				}

		}// end for

		return  fbxTextureInfo;
}