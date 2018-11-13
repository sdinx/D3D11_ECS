//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\FbxLoader.h>

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: FbxLoader::LoadTexcoords: インデックス座標でのUV値への対応.

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
		// FbxManager がない場合のみ生成
		CreateFbxManager();
		m_pScene = FbxScene::Create( s_pFbxManager, "scene" );
		m_pImporter = FbxImporter::Create( s_pFbxManager, "impoter" );
		m_pImporter->Initialize( szFileName.Buffer(), -1, s_pFbxManager->GetIOSettings() );

		if ( m_pImporter->Import( m_pScene ) == false )
		{
				Release();
				return;
		}

		LoadFbxModel( m_pScene );

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


void  FbxLoader::LoadFbxModel( FbxScene*  pScene )
{

		FbxMesh*  pMesh = nullptr;
		uint  nMeshCount = pScene->GetMemberCount<FbxMesh>();
		uint  nMaterialCount = pScene->GetMaterialCount();
		uint  vertexCount = 0;
		uint  k = 0, j = 0;
		uint  vertexIndex = 0;
		FbxGeometryElementNormal*  pNormals = nullptr;
		FbxGeometryElementUV*  pUVs = nullptr;
		std::vector<Vector3>  vertices;
		Vector3  vertex;

		// メッシュからデータを取得
		nMeshCount = pScene->GetMemberCount<FbxMesh>();
		m_modelContainer.resize( nMeshCount );
		for ( uint i = 0; i < nMeshCount; i++ )
		{
				pMesh = pScene->GetMember<FbxMesh>( i );
				m_modelContainer[i].pMesh = pMesh;
				m_modelContainer[i].indices = LoadIndices( pMesh );
				vertices = LoadVertices( pMesh );
				pNormals = pMesh->GetElementNormal();
				pUVs = pMesh->GetElementUV();

				// 三角ポリゴンの数ループ
				for ( j = 0; j < pMesh->GetPolygonCount(); j++ )
				{
						for ( k = 0; k < 3; k++ )
						{
								vertexIndex = pMesh->GetPolygonVertex( j, k );
								vertex = vertices[vertexIndex];
								// 法線とUVの取得
								m_modelContainer[i].normals.emplace_back( LoadNormal( pNormals, vertexIndex, vertexCount ) );
								m_modelContainer[i].texcoords.emplace_back( LoadTexcoord( pUVs, vertexIndex, pMesh->GetTextureUVIndex( j, k ) ) );

								m_modelContainer[i].vertices.emplace_back( vertex );
								vertexCount++;
						}// end for

				}// end for

				m_modelContainer[i].skinMeshes = LoadSkin( pMesh );
		}

		uint  c = 0;
		for ( uint i = 1; i < m_modelContainer.size(); i++ )
		{
				for ( c = 0; c < m_modelContainer[i].vertices.size(); c++ )
				{
						m_modelContainer[0].vertices.emplace_back( m_modelContainer[i].vertices[c] );
						m_modelContainer[0].normals.emplace_back( m_modelContainer[i].normals[c] );
						m_modelContainer[0].texcoords.emplace_back( m_modelContainer[i].texcoords[c] );
				}
		}

		m_materials.resize( nMaterialCount );
		for ( uint i = 0; i < nMaterialCount; i++ )
				m_materials[i] = LoadMaterial( pScene->GetMaterial( i ) );
}


std::vector<Vector3>  FbxLoader::LoadVertices( FbxMesh*  pMesh )
{
		std::vector<Vector3>  vertices;

		int  nCtrlPtCount = pMesh->GetControlPointsCount();
		FbxVector4  pos;
		vertices.reserve( nCtrlPtCount );

		for ( int i = 0; i < nCtrlPtCount; i++ )
		{
				pos = pMesh->GetControlPointAt( i );
				vertices.emplace_back( static_cast< float >( pos.mData[0] ), static_cast< float >( pos.mData[1] ), static_cast< float >( pos.mData[2] ) );
		}

		return  vertices;
}// end LoadVertices(FbxMesh*) : std::vector<Vector3>


Vector3  FbxLoader::LoadNormal( FbxGeometryElementNormal*&  pNormals, uint  vertexIndex, uint  vertexCount )
{
		Vector3  normal;
		FbxVector4  vec;
		int  index = 0;

		switch ( pNormals->GetMappingMode() )
		{
		case  FbxGeometryElement::eByControlPoint:
				switch ( pNormals->GetReferenceMode() )
				{
				case  FbxGeometryElement::eDirect:
						{
								vec = pNormals->GetDirectArray().GetAt( vertexIndex );
						}// case eDirect
						break;

				case  FbxGeometryElement::eIndexToDirect:
						{
								index = pNormals->GetIndexArray().GetAt( vertexIndex );
								vec = pNormals->GetDirectArray().GetAt( index );
						}// case eIndexToDirect
						break;

				}// end switch

		case  FbxGeometryElement::eByPolygonVertex:
				switch ( pNormals->GetReferenceMode() )
				{
				case  FbxGeometryElement::eDirect:
						{
								vec = pNormals->GetDirectArray().GetAt( vertexCount );
						}// case eDirect
						break;

				case  FbxGeometryElement::eIndexToDirect:
						{
								index = pNormals->GetIndexArray().GetAt( vertexCount );
								vec = pNormals->GetDirectArray().GetAt( index );
						}// case eIndexToDirect
						break;

				}// end switch

		}// end switch

		normal.m_floats[0] = static_cast< float >( vec.mData[0] );
		normal.m_floats[1] = static_cast< float >( vec.mData[1] );
		normal.m_floats[2] = static_cast< float >( vec.mData[2] );

		return  normal;
}


Vector2  FbxLoader::LoadTexcoord( FbxGeometryElementUV*&  pTexcoords, uint  vertexIndex, uint  texcoordIndex )
{
		Vector2  texcoord;
		FbxVector2  vec;
		int  index = 0;

		switch ( pTexcoords->GetMappingMode() )
		{
		case  FbxGeometryElement::eByControlPoint:
				switch ( pTexcoords->GetReferenceMode() )
				{
				case  FbxGeometryElement::eDirect:
						{
								vec = pTexcoords->GetDirectArray().GetAt( vertexIndex );
						}// case eDirect
						break;

				case  FbxGeometryElement::eIndexToDirect:
						{
								index = pTexcoords->GetIndexArray().GetAt( vertexIndex );
								vec = pTexcoords->GetDirectArray().GetAt( index );
						}// case eIndexToDirect
						break;

				}// end switch

		case  FbxGeometryElement::eByPolygonVertex:
				switch ( pTexcoords->GetReferenceMode() )
				{
				case  FbxGeometryElement::eDirect:
				case  FbxGeometryElement::eIndexToDirect:
						{
								vec = pTexcoords->GetDirectArray().GetAt( texcoordIndex );
						}// case eIndexToDirect | eDirect
						break;

				}// end switch

		}// end switch

		texcoord.x = static_cast< float >( vec.mData[0] );
		texcoord.y = 1.0f - static_cast< float >( vec.mData[1] );

		return  texcoord;
}


std::vector<INT>  FbxLoader::LoadIndices( FbxMesh*  pMesh )
{
		std::vector<INT>  indices;

		int polygonCount = pMesh->GetPolygonCount();// 三角面の数を取得.
		indices.reserve( polygonCount * 3 );// 頂点インデックス数の領域確保.

		// 面の構成情報を取得する
		for ( int i = 0; i < polygonCount; i++ )
		{
				indices.push_back( pMesh->GetPolygonVertex( i, 0 ) );// i番目の三角面の 0番目の頂点インデックス.
				indices.push_back( pMesh->GetPolygonVertex( i, 1 ) );// i番目の三角面の 1番目の頂点インデックス.
				indices.push_back( pMesh->GetPolygonVertex( i, 2 ) );// i番目の三角面の 2番目の頂点インデックス.
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

		prop = material->FindProperty( FbxSurfaceMaterial::sEmissive );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.emissive = Vector4( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ), static_cast< float >( color[3] ) );
		}

		return  mat;
}


SkinMesh  FbxLoader::LoadSkin( FbxMesh*  pMesh )
{
		SkinMesh  skin;

		const  auto  skinCount = pMesh->GetDeformerCount( FbxDeformer::eSkin );
		if ( skinCount == 0 )
				return  skin;// スケルタルアニメーションなし

		// 0番目のスキンデフォーマーを取得
		FbxSkin*  fbxSkin = static_cast< FbxSkin* >( pMesh->GetDeformer( 0, FbxDeformer::eSkin ) );

		const  auto  clusterCount = fbxSkin->GetClusterCount();
		if ( clusterCount == 0 )
				return  skin;

		skin.weights.resize( pMesh->GetPolygonVertexCount() );
		for ( auto& weight : skin.weights )
		{
				// サイズ確保と初期値代入
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
				cluster = fbxSkin->GetCluster( i );

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


ModelContainer  FbxLoader::SetAnimation( UINT  meshIndex, UINT  animationIndex, FbxTime&  fbxTime )
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