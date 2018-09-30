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

		// 三角化は処理が重いため予めしておく
		// FbxGeometryConverter  geometryConverter( s_pFbxManager );
		// geometryConverter.Triangulate( m_pScene, true );

		INT  nMeshCount = m_pScene->GetMemberCount<FbxMesh>();
		INT  nNodeCount = m_pScene->GetNodeCount();
		INT  nMaterialCount = m_pScene->GetMaterialCount();
		INT  i = 0;
		FbxNode*  pNode = nullptr;
		FbxMesh*  pMesh = nullptr;

		// メッシュからデータを取得
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
				index = indexList[i];// i番目の頂点情報（面の構成上情報順に並んでいる）を取得
				position = pMesh->GetControlPointAt( index );// index番目の頂点座標を取得
				if ( position[3] != 0.0 ) 
				{
						if ( position[3] == 1.0 )
						{
								vertices.push_back( Vector3( static_cast<float>( position[0] ), static_cast<float>( position[1] ), static_cast<float>( position[2] ) ) );// 同次座標化
						}// end if
						else 
						{
								vertices.push_back( Vector3( static_cast<float>( position[0] / position[3] ), static_cast<float>( position[1] / position[3] ), static_cast<float>( position[2] / position[3] ) ) );// 同次座標化
						}// end else
				}// end if
				else 
				{
						vertices.push_back( Vector3( static_cast<float>( position[0] ), static_cast<float>( position[1] ), static_cast<float>( position[2] ) ) );// W値を無視して格納
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
		INT  elementCount = pMesh->GetElementNormalCount();// 何個の法線情報がセットされているか
		INT  polygonSize = 0;

		assert( elementCount == 1 );// 1個の法線情報にしか対応しない

		FbxGeometryElementNormal* element = pMesh->GetElementNormal( 0 );// 0番目の法線セットを取得
		FbxLayerElement::EMappingMode mappingmode = element->GetMappingMode();// マッピングモード取得
		FbxLayerElement::EReferenceMode referrenceMode = element->GetReferenceMode();// リファレンスモード取得
		const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();// 法線情報を格納した頂点配列のインデックス配列を取得 
		const FbxLayerElementArrayTemplate<FbxVector4>& directArray = element->GetDirectArray();// 法線情報を格納した頂点配列を取得

		// eDirect または eIndexDirectのみ対応
		assert( ( referrenceMode == FbxGeometryElement::eDirect ) || ( referrenceMode == FbxGeometryElement::eIndexToDirect ) );

		normals.reserve( size );		// 頂点インデックス数分確保

																							// 頂点座標でマッピングされている場合
		if ( mappingmode == FbxGeometryElement::eByControlPoint ) 
		{
				for ( INT  i = 0; i < size; i++ ) 
				{
						index = indexList[i];// 面の構成情報配列のi番目の頂点インデックスを取得
						normalIndex = 0;	// 法線情報のインデックス

						// リファレンスモードを判定
						if ( referrenceMode == FbxGeometryElement::eDirect )// eDirectの場合
								normalIndex = index;// 頂点インデックスの位置に対応して保存されている
						else// eIndexToDirectの場合
								normalIndex = indexArray.GetAt( index );// 頂点座標インデックスに対応した法線情報VECTORのインデックスを取得

						normal = directArray.GetAt( normalIndex );// 法線情報を取得
						if ( normal[3] != 0.0f ) 
						{
								if ( normal[3] == 1.0f )
										normals.push_back( Vector3( static_cast<float>( normal[0] ), static_cast<float>( normal[1] ), static_cast<float>( normal[2] ) ) );// 同次座標へ
								else
										normals.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ), static_cast<float>( normal[1] / normal[3] ), static_cast<float>( normal[2] / normal[3] ) ) );// 同次座標へ

						}// end if
				}// end for
		}// end if
		else if ( mappingmode == FbxGeometryElement::eByPolygonVertex ) {
				// 頂点インデックス(面の構成情報)でマッピングされている場合
				int indexByPolygonVertex = 0;
				int polygonCount = pMesh->GetPolygonCount();	// ポリゴン数を取得

				for ( int i = 0; i < polygonCount; ++i ) 
				{
						polygonSize = pMesh->GetPolygonSize( i );	// 頂点数を取得
						normalIndex = 0;

						for ( j = 0; j < polygonSize; ++j ) 
						{
								// リファレンスモードを判定
								if ( referrenceMode == FbxGeometryElement::eDirect )// eDirectの場合
										normalIndex = indexByPolygonVertex;
								else
										normalIndex = indexArray.GetAt( indexByPolygonVertex );// eIndexToDirectの場合

								normal = directArray.GetAt( normalIndex );// 法線情報を取得
								if ( normal[3] != 0.0f ) 
								{
										if ( normal[3] == 1.0 )
												normals.push_back( Vector3( static_cast<float>( normal[0] ), static_cast<float>( normal[1] ), static_cast<float>( normal[2] ) ) );// 同次座標へ
										else
												normals.push_back( Vector3( static_cast<float>( normal[0] / normal[3] ), static_cast<float>( normal[1] / normal[3] ), static_cast<float>( normal[2] / normal[3] ) ) );// 同次座標へ
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
		INT  elementCount = pMesh->GetElementUVCount();// UV情報の数.

		// 一個のみに対応.
		if ( uvNo + 1 > elementCount )
				return  texcoords;

		INT  polygonCount = 0;
		INT  polygonSize = 0;
		INT  indexByPolygonVertex = 0;
		INT  j = 0;



		FbxGeometryElementUV* element = pMesh->GetElementUV( uvNo );// UVセットを取得.
		FbxLayerElement::EMappingMode mappingMode = element->GetMappingMode();// マッピングモードを取得.
		FbxLayerElement::EReferenceMode referenceMode = element->GetReferenceMode();	// リファレンスモードを取得	.
		const FbxLayerElementArrayTemplate<int>& indexArray = element->GetIndexArray();// UV情報を格納した頂点配列のインデックス配列を取得.
		const FbxLayerElementArrayTemplate<FbxVector2>& directArray = element->GetDirectArray();	// UV値配列を取得.

		// eDirectかeIndexDirectのみ対応.
		assert( ( referenceMode == FbxGeometryElement::eDirect ) || ( referenceMode == FbxGeometryElement::eIndexToDirect ) );
		
		// 頂点インデックス数を領域確保.
		texcoords.reserve( size );

		// 頂点に対応して格納されている場合.
		if ( mappingMode == FbxGeometryElement::eByControlPoint )
		{
				// 頂点座標でマッピング.
				for ( INT  i = 0; i < size; i++ )
				{
						index = indexList[i];// 面の構成情報配列から頂点インデックス番号を取得.

						uvIndex = 0;
						// リファレンスモードを判定.
						if ( referenceMode == FbxGeometryElement::eDirect )// eDirectの場合.
								uvIndex = index;// eDirectの場合（頂点インデックスと同じインデックス値でセットされている）
						else // eIndexToDirectの場合
								uvIndex = indexArray.GetAt( index );// 頂点座標インデックスに対応したＵＶ情報インデックスを取得.

						uv = directArray.GetAt( uvIndex );// uv値をdouble型で取得.
						texcoords.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );		// float値として格納.

				}// end for
		}// end if
		else if ( mappingMode == FbxGeometryElement::eByPolygonVertex )// 面の構成情報に対応して格納されている場合.
		{
				// 面の構成情報のインデックス でマッピング.
				indexByPolygonVertex = 0;// 面の構成情報インデックス配列のインデックス.
				polygonCount = pMesh->GetPolygonCount();// メッシュのポリゴン数を取得.

				for ( INT  i = 0; i < polygonCount; ++i )// ポリゴン数分ループ.
				{
						polygonSize = pMesh->GetPolygonSize( i );// i 番目のポリゴン頂点数を取得.

						// ポリゴンの頂点数分ループ
						for ( j = 0; j < polygonSize; ++j )
						{
								// リファレンスモードの判定?
								uvIndex = 0;
								if ( referenceMode == FbxGeometryElement::eDirect )// eDirectの場合.
										uvIndex = indexByPolygonVertex;// eDirectの場合
								else // eIndexToDirectの場合.
										uvIndex = indexArray.GetAt( indexByPolygonVertex );// 面の構成情報インデックスに対応したUV情報インデックスを取得.
								uv = directArray.GetAt( uvIndex );

								texcoords.push_back( Vector2( static_cast< float >( uv[0] ), static_cast< float >( uv[1] ) ) );// doubleからfloatへキャスト.

								++indexByPolygonVertex;// 頂点インデックスをインクリメント.
						}
				}
		}// end else if
		else// それ以外のマッピングモードには対応しない.
				assert( false );

		return  texcoords;
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