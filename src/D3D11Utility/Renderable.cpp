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
				// �e�N�X�`����NULL��
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
		int  indexByPolygonVertex = 0;						// �ʂ̍\�����C���f�b�N�X�z��̃C���f�b�N�X
		int  polygonCount = 0;

		// UV�Z�b�g�����擾
		int UVLayerCount = fbxMesh->GetElementUVCount();
		for ( int i = 0; UVLayerCount > i; i++ ) 
		{
				// UV�o�b�t�@���擾
				UV = fbxMesh->GetElementUV( i );
				indexArray = &UV->GetIndexArray();
				directArray = &UV->GetDirectArray();

				// �}�b�s���O���[�h�̎擾
				mapping = UV->GetMappingMode();
				// ���t�@�����X���[�h�̎擾
				reference = UV->GetReferenceMode();

				// UV�����擾
				uvCount = UV->GetDirectArray().GetCount();

				// ���_�ɑΉ����Ċi�[����Ă���ꍇ
				if ( mapping == FbxGeometryElement::eByControlPoint )
				{
						// ���_���W�Ń}�b�s���O
						for ( j = 0; j < size; j++ )
						{
								index = indexList[j];			// �ʂ̍\�����z�񂩂璸�_�C���f�b�N�X�ԍ����擾

																							// ���t�@�����X���[�h�𔻒�
								uvIndex;
								if ( reference == FbxGeometryElement::eDirect ) {		// eDirect�̏ꍇ
										uvIndex = index;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
								}
								else {													// eIndexToDirect�̏ꍇ
										uvIndex = indexArray->GetAt( index );				// ���_���W�C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
								}

								FbxVector2 uv = directArray->GetAt( uvIndex );		// uv�l��double�^�Ŏ擾
								fbxTextureInfo.texcoordList.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );		// float�l�Ƃ��Ċi�[
						}
				}
				// �ʂ̍\�����ɑΉ����Ċi�[����Ă���ꍇ
				else if ( mapping == FbxGeometryElement::eByPolygonVertex )
				{
						// �|���S���o�[�e�b�N�X�i�ʂ̍\�����̃C���f�b�N�X�j�Ń}�b�s���O
						indexByPolygonVertex = 0;
						polygonCount = fbxMesh->GetPolygonCount();			// ���b�V���̃|���S�������擾
						for ( j = 0; j < polygonCount; ++j )				// �|���S���������[�v
						{
								polygonSize = fbxMesh->GetPolygonSize( j );		// ���Ԗڂ̃|���S�����_�����擾

																																// �|���S���̒��_�������[�v
								for ( k = 0; k < polygonSize; ++k )
								{
										// ���t�@�����X���[�h�̔���H
										if ( reference == FbxGeometryElement::eDirect ) {		// eDirect�̏ꍇ
												uvIndex = indexByPolygonVertex;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
										}
										else {													// eIndexToDirect�̏ꍇ
												uvIndex = indexArray->GetAt( indexByPolygonVertex );	// �ʂ̍\�����C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
										}
										uv = directArray->GetAt( uvIndex );

										fbxTextureInfo.texcoordList.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );	// �����������l�Ƃ��Ċi�[

										++indexByPolygonVertex;						// ���_�C���f�b�N�X���C���N�������g
								}
						}
				}
				else
				{
						// ����ȊO�̃}�b�s���O���[�h�ɂ͑Ή����Ȃ�
						assert( false );
				}

		}// end for

		return  fbxTextureInfo;
}