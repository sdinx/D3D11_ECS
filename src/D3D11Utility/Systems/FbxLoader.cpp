//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\FbxLoader.h>


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

		// �O�p��
		FbxGeometryConverter  geometryConverter( s_pFbxManager );
		geometryConverter.Triangulate( m_pScene, true );

		INT  nMeshCount = m_pScene->GetMemberCount<FbxMesh>();
		INT  nNodeCount = m_pScene->GetNodeCount();
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
		}

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
				index = indexList[i];						// i�Ԗڂ̒��_���i�ʂ̍\�����񏇂ɕ���ł���j���擾
				position = pMesh->GetControlPointAt( index );		// index�Ԗڂ̒��_���W���擾
				if ( position[3] != 0.0 ) 
				{
						if ( position[3] == 1.0 )
						{
								vertices.push_back( Vector3( static_cast<float>( position[0] ), static_cast<float>( position[1] ), static_cast<float>( position[2] ) ) );			// �������W��
						}// end if
						else 
						{
								vertices.push_back( Vector3( static_cast<float>( position[0] / position[3] ), static_cast<float>( position[1] / position[3] ), static_cast<float>( position[2] / position[3] ) ) );		// �������W��
						}// end else
				}// end if
				else 
				{
						vertices.push_back( Vector3( static_cast<float>( position[0] ), static_cast<float>( position[1] ), static_cast<float>( position[2] ) ) );				// �v�𖳎����Ċi�[
				}// end else
		}// end for

		return  vertices;
}// end LoadVertices(FbxMesh*) : std::vector<Vector3>


std::vector<Vector3>  FbxLoader::LoadNormals( FbxMesh*  pMesh, INT  size )
{
		std::vector<Vector3>  normals;

		INT  index = 0;
		int  normalIndex = 0;
		int  j = 0;
		FbxVector4  normal;
		INT*  indexList = pMesh->GetPolygonVertices();
		int  elementCount = pMesh->GetElementNormalCount();			// ���̖@����񂪃Z�b�g����Ă��邩
		int  polygonSize = 0;

		assert( elementCount == 1 );					// �P�̖@�����ɂ����Ή����Ȃ�

		FbxGeometryElementNormal* element = pMesh->GetElementNormal( 0 );	// �O�Ԗڂ̖@���Z�b�g���擾
		FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();	// �}�b�s���O���[�h�擾
		FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h�擾
		const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �@�������i�[�����������������z��̃C���f�b�N�X�l�z����擾 
		const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();	// �@�������i�[�����������������z����擾

																																															// eDirect���@eIndexDirect�̂ݑΉ�
		assert( ( referrenceMode == FbxGeometryElement::eDirect ) || ( referrenceMode == FbxGeometryElement::eIndexToDirect ) );

		normals.reserve( size );		// ���_�C���f�b�N�X�����m��

																							// ���_���W�Ń}�b�s���O����Ă���ꍇ
		if ( mappingmode == FbxGeometryElement::eByControlPoint ) {
				for ( INT  i = 0; i < size; i++ ) {
						index = indexList[i];		// �ʂ̍\�����z���i�Ԗڂ̒��_�C���f�b�N�X���擾
						normalIndex = 0;				// �@�����̃C���f�b�N�X
																		// ���t�@�����X���[�h�𔻒�
						if ( referrenceMode == FbxGeometryElement::eDirect ) {		// eDirect�̏ꍇ
								normalIndex = index;								// ���_�C���f�b�N�X�̈ʒu�ɑΉ����ĕۑ�����Ă���
						}
						else {													// eIndexToDirect�̏ꍇ
								normalIndex = indexArray.GetAt( index );				// ���_���W�C���f�b�N�X�ɑΉ������@�����VECTOR�̃C���f�b�N�X���擾
						}
						normal = directArray.GetAt( normalIndex );		// �@�������擾
						if ( normal[3] != 0.0f ) {
								if ( normal[3] == 1.0f ) {
										normals.push_back( Vector3( static_cast<float>( normal[0] ), static_cast<float>( normal[1] ), static_cast<float>( normal[2] ) ) );			// �������W��
								}
								else {
										normals.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ), static_cast<float>( normal[1] / normal[3] ), static_cast<float>( normal[2] / normal[3] ) ) );		// �������W��
								}
						}
				}
		}
		else if ( mappingmode == FbxGeometryElement::eByPolygonVertex ) {
				// ���_�C���f�b�N�X(�ʂ̍\�����)�Ń}�b�s���O����Ă���ꍇ
				int indexByPolygonVertex = 0;
				int polygonCount = pMesh->GetPolygonCount();					// �|���S�������擾
				for ( int i = 0; i < polygonCount; ++i ) {
						polygonSize = pMesh->GetPolygonSize( i );				// ���_�����擾
						normalIndex = 0;
						for ( j = 0; j < polygonSize; ++j ) {
								// ���t�@�����X���[�h�𔻒�
								if ( referrenceMode == FbxGeometryElement::eDirect ) {		// eDirect�̏ꍇ
										normalIndex = indexByPolygonVertex;
								}
								else {
										normalIndex = indexArray.GetAt( indexByPolygonVertex );	// eIndexToDirect�̏ꍇ
								}
								normal = directArray.GetAt( normalIndex );		// �@�������擾
								if ( normal[3] != 0.0f ) {
										if ( normal[3] == 1.0 ) {
												normals.push_back( Vector3( static_cast<float>( normal[0] ), static_cast<float>( normal[1] ), static_cast<float>( normal[2] ) ) );		// �������W��
										}
										else {
												normals.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ), static_cast<float>( normal[1] / normal[3] ), static_cast<float>( normal[2] / normal[3] ) ) );	// �������W��
										}
								}
								++indexByPolygonVertex;
						}
				}
		}
		else {
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
		INT  elementCount = pMesh->GetElementUVCount();		// ���̂t�u��񂪃Z�b�g����Ă��邩
		if ( uvNo + 1 > elementCount )						// ����� 1�ɂ̂ݑΉ�
		{
				return  texcoords;
		}
		INT  polygonCount = 0;
		INT  polygonSize = 0;
		INT  indexByPolygonVertex = 0;
		INT  j = 0;

		FbxGeometryElementUV* element = pMesh->GetElementUV( uvNo );					// UV�Z�b�g���擾
		FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();		// �}�b�s���O���[�h���擾
		FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// ���t�@�����X���[�h���擾	
		const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();	// �t�u�����i�[�����u�d�b�s�n�q�z��̃C���f�b�N�X�z����擾
		const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// �t�u�l�z����擾

																																															// eDirct��eIndexDirect�̂ݑΉ�
		assert( ( referenceMode == FbxGeometryElement::eDirect ) || ( referenceMode == FbxGeometryElement::eIndexToDirect ) );

		// ���_�C���f�b�N�X�����������������e�ʂ��m��
		texcoords.reserve( size );

		// ���_�ɑΉ����Ċi�[����Ă���ꍇ
		if ( mappingMode == FbxGeometryElement::eByControlPoint )
		{
				// ���_���W�Ń}�b�s���O
				for ( INT  i = 0; i < size; i++ )
				{
						index = indexList[i];			// �ʂ̍\�����z�񂩂璸�_�C���f�b�N�X�ԍ����擾

																					// ���t�@�����X���[�h�𔻒�
						uvIndex = 0;
						if ( referenceMode == FbxGeometryElement::eDirect ) {		// eDirect�̏ꍇ
								uvIndex = index;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
						}
						else {													// eIndexToDirect�̏ꍇ
								uvIndex = indexArray.GetAt( index );				// ���_���W�C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
						}

						uv = directArray.GetAt( uvIndex );		// uv�l��double�^�Ŏ擾
						texcoords.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );		// float�l�Ƃ��Ċi�[
				}
		}
		// �ʂ̍\�����ɑΉ����Ċi�[����Ă���ꍇ
		else if ( mappingMode == FbxGeometryElement::eByPolygonVertex )
		{
				// �|���S���o�[�e�b�N�X�i�ʂ̍\�����̃C���f�b�N�X�j�Ń}�b�s���O
				indexByPolygonVertex = 0;						// �ʂ̍\�����C���f�b�N�X�z��̃C���f�b�N�X
				polygonCount = pMesh->GetPolygonCount();			// ���b�V���̃|���S�������擾
				for ( INT  i = 0; i < polygonCount; ++i )				// �|���S���������[�v
				{
						polygonSize = pMesh->GetPolygonSize( i );		// ���Ԗڂ̃|���S�����_�����擾

																														// �|���S���̒��_�������[�v
						for ( j = 0; j < polygonSize; ++j )
						{
								// ���t�@�����X���[�h�̔���H
								uvIndex = 0;
								if ( referenceMode == FbxGeometryElement::eDirect ) {		// eDirect�̏ꍇ
										uvIndex = indexByPolygonVertex;		//�@eDirect�̏ꍇ�i���_�C���f�b�N�X�Ɠ����C���f�b�N�X�l�ŃZ�b�g����Ă���j
								}
								else {													// eIndexToDirect�̏ꍇ
										uvIndex = indexArray.GetAt( indexByPolygonVertex );	// �ʂ̍\�����C���f�b�N�X�ɑΉ������t�u���C���f�b�N�X���擾
								}
								uv = directArray.GetAt( uvIndex );

								texcoords.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );	// �����������l�Ƃ��Ċi�[

								++indexByPolygonVertex;						// ���_�C���f�b�N�X���C���N�������g
						}
				}
		}
		else
		{
				// ����ȊO�̃}�b�s���O���[�h�ɂ͑Ή����Ȃ�
				assert( false );
		}

		return  texcoords;
}


std::vector<INT>  FbxLoader::LoadIndices( FbxMesh*  pMesh )
{
		std::vector<INT>  indices;

		int polygonCount = pMesh->GetPolygonCount();			// �O�p�`�̐����擾
		indices.reserve( polygonCount * 3 );				// 3�p�`�̐��~�R

																									// �ʂ̍\�������擾����
		for ( int i = 0; i < polygonCount; i++ ) {
				indices.push_back( pMesh->GetPolygonVertex( i, 0 ) );		// i�Ԗڂ̎O�p�`�̂O�Ԗڂ̒��_�C���f�b�N�X���擾
				indices.push_back( pMesh->GetPolygonVertex( i, 1 ) );		// i�Ԗڂ̎O�p�`�̂P�Ԗڂ̒��_�C���f�b�N�X���擾
				indices.push_back( pMesh->GetPolygonVertex( i, 2 ) );		// i�Ԗڂ̎O�p�`�̂Q�Ԗڂ̒��_�C���f�b�N�X���擾
		}

		return  indices;
}


ModelContainer  FbxLoader::LoadMesh( FbxScene*  pScene )
{
		ModelContainer  modelMesh;

		return  modelMesh;
}


void  FbxLoader::Release()
{
		m_modelContainer.clear();
		SafeDestroy( m_pImporter );
		SafeDestroy( m_pScene );
}