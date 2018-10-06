//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\FbxLoader.h>

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: FbxLoader::LoadTexcoords: �C���f�b�N�X���W�ł�UV�l�ւ̑Ή�.

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Systems;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
FbxManager*  FbxLoader::s_pFbxManager = nullptr;


FbxLoader::FbxLoader() :
		m_pScene( nullptr ),
		m_pImporter( nullptr )
{
		CreateFbxManager();
}


FbxLoader::FbxLoader( FbxString  szFileName ) :
		m_pScene( nullptr ),
		m_pImporter( nullptr )
{
		// FbxManager ���Ȃ��ꍇ�̂ݐ���
		CreateFbxManager();
		m_pScene = FbxScene::Create( s_pFbxManager, "scene" );
		m_pImporter = FbxImporter::Create( s_pFbxManager, "impoter" );
		m_pImporter->Initialize( szFileName.Buffer(), -1, s_pFbxManager->GetIOSettings() );
		
		if ( m_pImporter->Import( m_pScene ) == false )
		{
				Release();
				return;
		}

		// �O�p���͏������d�����ߗ\�߂��Ă���
		// FbxGeometryConverter  geometryConverter( s_pFbxManager );
		// geometryConverter.Triangulate( m_pScene, true );

		INT  nMeshCount = m_pScene->GetMemberCount<FbxMesh>();
		INT  nNodeCount = m_pScene->GetNodeCount();
		INT  nMaterialCount = m_pScene->GetMaterialCount();
		INT  i = 0;
		FbxNode*  pNode = nullptr;
		FbxMesh*  pMesh = nullptr;

		// ���b�V������f�[�^���擾
		nMeshCount = m_pScene->GetMemberCount<FbxMesh>();
		m_modelContainer.resize( nMeshCount );
		for ( i = 0; i < nMeshCount; i++ )
		{
				pMesh = m_pScene->GetMember<FbxMesh>( i );
				m_modelContainer[i].pMesh = pMesh;
				m_modelContainer[i].indices = LoadIndices( pMesh );
				m_modelContainer[i].vertices = LoadVertices( pMesh, m_modelContainer[i].indices.size() );
				m_modelContainer[i].normals = LoadNormals( pMesh, m_modelContainer[i].indices.size() );
				m_modelContainer[i].texcoords = LoadTexcoords( pMesh, m_modelContainer[i].indices.size() );
				m_modelContainer[i].skinMeshes = LoadSkin( pMesh );
		}

		m_materials.resize( nMaterialCount );
		for ( i = 0; i < nMaterialCount; i++ )
				m_materials[i] = LoadMaterial( m_pScene->GetMaterial( i ) );

}


FbxLoader::~FbxLoader()
{
		Release();
}


void  FbxLoader::CreateFbxManager()
{
		if ( s_pFbxManager == nullptr )
				s_pFbxManager = FbxManager::Create();
}


std::vector<Vector3>  FbxLoader::LoadVertices( FbxMesh*  pMesh, INT  size )
{
		std::vector<Vector3>  vertices;

		INT  index = 0;
		INT*  indexList = pMesh->GetPolygonVertices();
		FbxVector4  position;
		vertices.reserve( size );

		for ( INT  i = 0; i < size; i++ )
		{
				index = indexList[i];// i�Ԗڂ̒��_���i�ʂ̍\�����񏇂ɕ���ł���j���擾
				position = pMesh->GetControlPointAt( index );// index�Ԗڂ̒��_���W���擾
				if ( position[3] != 0.0 ) 
				{
						if ( position[3] == 1.0 )
						{
								vertices.push_back( Vector3( static_cast<float>( position[0] ), static_cast<float>( position[1] ), static_cast<float>( position[2] ) ) );// �������W��
						}// end if
						else 
						{
								vertices.push_back( Vector3( static_cast<float>( position[0] / position[3] ), static_cast<float>( position[1] / position[3] ), static_cast<float>( position[2] / position[3] ) ) );// �������W��
						}// end else
				}// end if
				else 
				{
						vertices.push_back( Vector3( static_cast<float>( position[0] ), static_cast<float>( position[1] ), static_cast<float>( position[2] ) ) );// W�l�𖳎����Ċi�[
				}// end else
		}// end for

		return  vertices;
}// end LoadVertices(FbxMesh*) : std::vector<Vector3>


std::vector<Vector3>  FbxLoader::LoadNormals( FbxMesh*  pMesh, INT  size )
{
		std::vector<Vector3>  normals;

		INT  index = 0;
		INT  normalIndex = 0;
		INT  j = 0;
		FbxVector4  normal;
		INT*  indexList = pMesh->GetPolygonVertices();
		INT  elementCount = pMesh->GetElementNormalCount();// ���̖@����񂪃Z�b�g����Ă��邩
		INT  polygonSize = 0;

		assert( elementCount == 1 );// 1�̖@�����ɂ����Ή����Ȃ�

		FbxGeometryElementNormal* element = pMesh->GetElementNormal( 0 );// 0�Ԗڂ̖@���Z�b�g���擾
		FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();// �}�b�s���O���[�h�擾
		FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();// ���t�@�����X���[�h�擾
		const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();// �@�������i�[�������_�z��̃C���f�b�N�X�z����擾 
		const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();// �@�������i�[�������_�z����擾

		// eDirect �܂��� eIndexDirect�̂ݑΉ�
		assert( ( referrenceMode == FbxGeometryElement::eDirect ) || ( referrenceMode == FbxGeometryElement::eIndexToDirect ) );

		normals.reserve( size );		// ���_�C���f�b�N�X�����m��

																							// ���_���W�Ń}�b�s���O����Ă���ꍇ
		if ( mappingmode == FbxGeometryElement::eByControlPoint ) 
		{
				for ( INT  i = 0; i < size; i++ ) 
				{
						index = indexList[i];// �ʂ̍\�����z���i�Ԗڂ̒��_�C���f�b�N�X���擾
						normalIndex = 0;	// �@�����̃C���f�b�N�X

						// ���t�@�����X���[�h�𔻒�
						if ( referrenceMode == FbxGeometryElement::eDirect )// eDirect�̏ꍇ
								normalIndex = index;// ���_�C���f�b�N�X�̈ʒu�ɑΉ����ĕۑ�����Ă���
						else// eIndexToDirect�̏ꍇ
								normalIndex = indexArray.GetAt( index );// ���_���W�C���f�b�N�X�ɑΉ������@�����VECTOR�̃C���f�b�N�X���擾

						normal = directArray.GetAt( normalIndex );// �@�������擾
						if ( normal[3] != 0.0f ) 
						{
								if ( normal[3] == 1.0f )
										normals.push_back( Vector3( static_cast<float>( normal[0] ), static_cast<float>( normal[1] ), static_cast<float>( normal[2] ) ) );// �������W��
								else
										normals.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ), static_cast<float>( normal[1] / normal[3] ), static_cast<float>( normal[2] / normal[3] ) ) );// �������W��

						}// end if
				}// end for
		}// end if
		else if ( mappingmode == FbxGeometryElement::eByPolygonVertex ) {
				// ���_�C���f�b�N�X(�ʂ̍\�����)�Ń}�b�s���O����Ă���ꍇ
				int indexByPolygonVertex = 0;
				int polygonCount = pMesh->GetPolygonCount();	// �|���S�������擾

				for ( int i = 0; i < polygonCount; ++i ) 
				{
						polygonSize = pMesh->GetPolygonSize( i );	// ���_�����擾
						normalIndex = 0;

						for ( j = 0; j < polygonSize; ++j ) 
						{
								// ���t�@�����X���[�h�𔻒�
								if ( referrenceMode == FbxGeometryElement::eDirect )// eDirect�̏ꍇ
										normalIndex = indexByPolygonVertex;
								else
										normalIndex = indexArray.GetAt( indexByPolygonVertex );// eIndexToDirect�̏ꍇ

								normal = directArray.GetAt( normalIndex );// �@�������擾
								if ( normal[3] != 0.0f ) 
								{
										if ( normal[3] == 1.0 )
												normals.push_back( Vector3( static_cast<float>( normal[0] ), static_cast<float>( normal[1] ), static_cast<float>( normal[2] ) ) );// �������W��
										else
												normals.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ), static_cast<float>( normal[1] / normal[3] ), static_cast<float>( normal[2] / normal[3] ) ) );// �������W��
								}// end if
								++indexByPolygonVertex;

						}// end for
				}// end for
		}// end else if
		else 
		{
				assert( false );
		}

		return  normals;
}


std::vector<Vector2>  FbxLoader::LoadTexcoords( FbxMesh*  pMesh, INT  size )
{
		std::vector<Vector2>  texcoords;

		const  INT  uvNo = 0;
		INT  index = 0;
		INT  uvIndex = 0;
		FbxVector2  uv;
		INT*  indexList = pMesh->GetPolygonVertices();
		INT  elementCount = pMesh->GetElementUVCount();// UV���̐�.

		// ��݂̂ɑΉ�.
		if ( uvNo + 1 > elementCount )
				return  texcoords;

		INT  polygonCount = 0;
		INT  polygonSize = 0;
		INT  indexByPolygonVertex = 0;
		INT  j = 0;



		FbxGeometryElementUV* element = pMesh->GetElementUV( uvNo );// UV�Z�b�g���擾.
		FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();// �}�b�s���O���[�h���擾.
		FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h���擾	.
		const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();// UV�����i�[�������_�z��̃C���f�b�N�X�z����擾.
		const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// UV�l�z����擾.

		// eDirect��eIndexDirect�̂ݑΉ�.
		assert( ( referenceMode == FbxGeometryElement::eDirect ) || ( referenceMode == FbxGeometryElement::eIndexToDirect ) );
		
		// ���_�C���f�b�N�X����̈�m��.
		texcoords.reserve( size );

		// ���_�ɑΉ����Ċi�[����Ă���ꍇ.
		if ( mappingMode == FbxGeometryElement::eByControlPoint )
		{
				// ���_���W�Ń}�b�s���O.
				for ( INT  i = 0; i < size; i++ )
				{
						index = indexList[i];// �ʂ̍\�����z�񂩂璸�_�C���f�b�N�X�ԍ����擾.

						uvIndex = 0;
						// ���t�@�����X���[�h�𔻒�.
						if ( referenceMode == FbxGeometryElement::eDirect )// eDirect�̏ꍇ.
								uvIndex = index;// eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
						else // eIndexToDirect�̏ꍇ
								uvIndex = indexArray.GetAt( index );// ���_���W�C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾.

						uv = directArray.GetAt( uvIndex );// uv�l��double�^�Ŏ擾.
						texcoords.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );		// float�l�Ƃ��Ċi�[.

				}// end for
		}// end if
		else if ( mappingMode == FbxGeometryElement::eByPolygonVertex )// �ʂ̍\�����ɑΉ����Ċi�[����Ă���ꍇ.
		{
				// �ʂ̍\�����̃C���f�b�N�X �Ń}�b�s���O.
				indexByPolygonVertex = 0;// �ʂ̍\�����C���f�b�N�X�z��̃C���f�b�N�X.
				polygonCount = pMesh->GetPolygonCount();// ���b�V���̃|���S�������擾.

				for ( INT  i = 0; i < polygonCount; ++i )// �|���S���������[�v.
				{
						polygonSize = pMesh->GetPolygonSize( i );// i �Ԗڂ̃|���S�����_�����擾.

						// �|���S���̒��_�������[�v
						for ( j = 0; j < polygonSize; ++j )
						{
								// ���t�@�����X���[�h�̔���?
								uvIndex = 0;
								if ( referenceMode == FbxGeometryElement::eDirect )// eDirect�̏ꍇ.
										uvIndex = indexByPolygonVertex;// eDirect�̏ꍇ
								else // eIndexToDirect�̏ꍇ.
										uvIndex = indexArray.GetAt( indexByPolygonVertex );// �ʂ̍\�����C���f�b�N�X�ɑΉ�����UV���C���f�b�N�X���擾.
								uv = directArray.GetAt( uvIndex );

								texcoords.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );// double����float�փL���X�g.

								++indexByPolygonVertex;// ���_�C���f�b�N�X���C���N�������g.
						}
				}
		}// end else if
		else// ����ȊO�̃}�b�s���O���[�h�ɂ͑Ή����Ȃ�.
				assert( false );

		return  texcoords;
}


std::vector<INT>  FbxLoader::LoadIndices( FbxMesh*  pMesh )
{
		std::vector<INT>  indices;

		int polygonCount = pMesh->GetPolygonCount();// �O�p�ʂ̐����擾.
		indices.reserve( polygonCount * 3 );// ���_�C���f�b�N�X���̗̈�m��.

		// �ʂ̍\�������擾����
		for ( int i = 0; i < polygonCount; i++ ) 
		{
				indices.push_back( pMesh->GetPolygonVertex( i, 0 ) );// i�Ԗڂ̎O�p�ʂ� 0�Ԗڂ̒��_�C���f�b�N�X.
				indices.push_back( pMesh->GetPolygonVertex( i, 1 ) );// i�Ԗڂ̎O�p�ʂ� 1�Ԗڂ̒��_�C���f�b�N�X.
				indices.push_back( pMesh->GetPolygonVertex( i, 2 ) );// i�Ԗڂ̎O�p�ʂ� 2�Ԗڂ̒��_�C���f�b�N�X.
		}

		return  indices;
}


Material  FbxLoader::LoadMaterial( FbxSurfaceMaterial*  material )
{
		Material  mat;
		FbxVector4  color;
		FbxProperty  prop;

		prop = material->FindProperty( FbxSurfaceMaterial::sDiffuse );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.diffuse = Vector4( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ), static_cast< float >( color[3] ) );
		}

		prop = material->FindProperty( FbxSurfaceMaterial::sAmbient );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.ambient = Vector4( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ), static_cast< float >( color[3] ) );
		}

		prop = material->FindProperty( FbxSurfaceMaterial::sSpecular );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.specular = Vector4( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ), static_cast< float >( color[3] ) );
		}

		prop = material->FindProperty( FbxSurfaceMaterial::sShininess );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.shininess = Vector4( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ), static_cast< float >( color[3] ) );
		}

		return  mat;
}


SkinMesh  FbxLoader::LoadSkin( FbxMesh*  pMesh )
{
		SkinMesh  skin;

		const  auto  skinCount = pMesh->GetDeformerCount( FbxDeformer::eSkin );
		if ( skinCount == 0 )
				return  skin;// �X�P���^���A�j���[�V�����Ȃ�

		// 0�Ԗڂ̃X�L���f�t�H�[�}�[���擾
		FbxSkin*  fbxSkin = static_cast< FbxSkin* >( pMesh->GetDeformer( 0, FbxDeformer::eSkin ) );
		
		const  auto  clusterCount = fbxSkin->GetClusterCount();
		if ( clusterCount == 0 )
				return  skin;

		skin.weights.resize( pMesh->GetPolygonVertexCount() );
		for ( auto& weight : skin.weights )
		{
				// �T�C�Y�m�ۂƏ����l���
				weight.resize( clusterCount, 0.0f );
		}// end for

		skin.base_inverse.resize( clusterCount );

		const  auto  vtxIndexCount = pMesh->GetPolygonVertexCount();
		const  int*  vtxIndices = pMesh->GetPolygonVertices();

		int  k = 0, j = 0;
		int  indexCount = 0;
		int*  indices = nullptr;
		double*  weights = nullptr;
		float  w = 0;
		FbxCluster*  cluster = nullptr;

		for ( int i = 0; i < clusterCount; i++ )
		{
				cluster = fbxSkin->GetCluster(i);

				assert( cluster->GetLinkMode() == FbxCluster::eNormalize );

				indexCount = cluster->GetControlPointIndicesCount();
				indices = cluster->GetControlPointIndices();
				weights = cluster->GetControlPointWeights();

				for ( j = 0; j < indexCount; j++ )
				{
						w = static_cast< float >( weights[j] );
						for ( k = 0; k < vtxIndexCount; k++ )
								if ( vtxIndices[k] == indices[j] )
										skin.weights[k][i] = w;

				}//end for
		}// end for

		return  skin;
}


ModelContainer  FbxLoader::LoadMesh( FbxScene*  pScene )
{
		ModelContainer  modelMesh;

		return  modelMesh;
}


ModelContainer  FbxLoader::SetAnimation(  UINT  meshIndex, UINT  animationIndex, FbxTime&  fbxTime )
{
		FbxSkin*  fbxSkin = static_cast< FbxSkin* >( m_modelContainer[meshIndex].pMesh->GetDeformer( 0, FbxDeformer::eSkin ) );
		const  int  clusterCount = fbxSkin->GetClusterCount();
		FbxCluster*  cluster = nullptr;
		FbxNode*  pNode = nullptr;
		FbxAMatrix  mtxClusterGlobalCurrentPosition;

		for ( int i = 0; i < clusterCount; i++ )
		{
				cluster = fbxSkin->GetCluster( i );

				pNode = cluster->GetLink();
				mtxClusterGlobalCurrentPosition = pNode->EvaluateGlobalTransform( fbxTime );


		}

		return  ModelContainer();
}


void  FbxLoader::Release()
{
		m_modelContainer.clear();
		SafeDestroy( m_pImporter );
		SafeDestroy( m_pScene );
}