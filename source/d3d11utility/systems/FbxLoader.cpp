//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\FbxLoader.h>
#include  <iostream>

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
		m_pImporter( nullptr ),
		m_isAnimation( false )
{
		CreateFbxManager();
}


FbxLoader::FbxLoader( FbxString  fileName ) :
		m_pScene( nullptr ),
		m_pImporter( nullptr ),
		m_isAnimation( false )
{
		// FbxManager がない場合のみ生成
		CreateFbxManager();
		m_pScene = FbxScene::Create( s_pFbxManager, "scene" );
		m_pImporter = FbxImporter::Create( s_pFbxManager, "impoter" );
		m_pImporter->Initialize( fileName.Buffer(), -1, s_pFbxManager->GetIOSettings() );

		if ( m_pImporter->Import( m_pScene ) == false )
		{
				m_pImporter->Destroy();
				m_pImporter = nullptr;
				Release();
				return;
		}

		m_pImporter->Destroy();
		m_pImporter = nullptr;
		LoadFbxModel( m_pScene );

		m_fileName = fileName;
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
		LoadSkeltonHierarchy( pScene->GetRootNode() );
		if ( m_skeleton.joints.empty() == false )
				m_isAnimation = true;

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
				pNormals = pMesh->GetElementNormal();
				pUVs = pMesh->GetElementUV();
				m_modelContainer[i].pMesh = pMesh;
				m_modelContainer[i].indices = LoadIndices( pMesh );
				vertices = LoadVertices( pMesh );
				vertexCount = 0;

				// 三角ポリゴンの数ループ
				for ( j = 0; j < ( uint ) pMesh->GetPolygonCount(); j++ )
				{
						for ( k = 0; k < 3; k++ )
						{
								vertexIndex = pMesh->GetPolygonVertex( j, k );
								vertex = vertices[vertexIndex];
								// 法線とUVの取得
								m_modelContainer[i].normals.push_back( LoadNormal( pNormals, vertexIndex, vertexCount ) );
								m_modelContainer[i].texcoords.push_back( LoadTexcoord( pUVs, vertexIndex, pMesh->GetTextureUVIndex( j, k ) ) );

								m_modelContainer[i].vertices.push_back( vertex );
								vertexCount++;
						}// end for

				}// end for

				m_modelContainer[i].skinMeshes = LoadSkin( pMesh );
		}

		uint  c = 0, d = 0;
		for ( uint i = 1; i < m_modelContainer.size(); i++ )
		{
				for ( c = 0; c < m_modelContainer[i].vertices.size(); c++ )
				{
						m_modelContainer[0].vertices.emplace_back( m_modelContainer[i].vertices[c] );
						m_modelContainer[0].normals.emplace_back( m_modelContainer[i].normals[c] );
						m_modelContainer[0].texcoords.emplace_back( m_modelContainer[i].texcoords[c] );
				}
		}

		for ( uint i = 0; i < nMaterialCount; i++ )
		{
				m_materials.emplace_back( LoadMaterial( pScene->GetMaterial( i ) ) );
		}
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


		//if ( vec.mData[3] != 0.0f || vec.mData[3] == 1.0f )
		//{
		//		normal.m_floats[0] = static_cast< float >( vec.mData[0] );
		//		normal.m_floats[1] = static_cast< float >( vec.mData[1] );
		//		normal.m_floats[2] = static_cast< float >( vec.mData[2] );
		//}// end if
		//else
		//{
		//		normal.m_floats[0] = static_cast< float >( vec.mData[0] / vec.mData[3] );
		//		normal.m_floats[1] = static_cast< float >( vec.mData[1] / vec.mData[3] );
		//		normal.m_floats[2] = static_cast< float >( vec.mData[2] / vec.mData[3] );
		//}// end else


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
		for ( int i = 0; i < polygonCount; i++ ) {
				indices.push_back( pMesh->GetPolygonVertex( i, 0 ) );// i番目の三角面の 0番目の頂点インデックス.
				indices.push_back( pMesh->GetPolygonVertex( i, 1 ) );// i番目の三角面の 1番目の頂点インデックス.
				indices.push_back( pMesh->GetPolygonVertex( i, 2 ) );// i番目の三角面の 2番目の頂点インデックス.
		}

		return  indices;
}


Graphics::Material  FbxLoader::LoadMaterial( FbxSurfaceMaterial*  material )
{
		Graphics::Material  mat;
		FbxVector4  color;
		FbxProperty  prop;

		prop = material->FindProperty( FbxSurfaceMaterial::sDiffuse );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.SetDiffuse( Vector3( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ) ) );
		}

		prop = material->FindProperty( FbxSurfaceMaterial::sAmbient );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.SetAmbient( Vector3( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ) ) );
		}

		prop = material->FindProperty( FbxSurfaceMaterial::sSpecular );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.SetSpecular( Vector4( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ), static_cast< float >( color[3] ) ) );
		}

		prop = material->FindProperty( FbxSurfaceMaterial::sEmissive );
		if ( prop.IsValid() )
		{
				color = prop.Get<FbxVector4>();
				mat.SetEmissive( Vector4( static_cast< float >( color[0] ), static_cast< float >( color[1] ), static_cast< float >( color[2] ), static_cast< float >( color[3] ) ) );
		}

		return  mat;
}


SkinMesh  FbxLoader::LoadSkin( FbxMesh*  pMesh )
{
		SkinMesh  skin;
		FbxNode* pNode = pMesh->GetNode();
		WeightPair  weightPair;

		FbxAMatrix  geometryTransform(
				pNode->GetGeometricTranslation( FbxNode::eSourcePivot ),
				pNode->GetGeometricRotation( FbxNode::eSourcePivot ),
				pNode->GetGeometricScaling( FbxNode::eSourcePivot )
		);

		const  auto  nSkinCounts = pMesh->GetDeformerCount( FbxDeformer::eSkin );
		if ( nSkinCounts == 0 )
				return  skin;// スケルタルアニメーションなし

		// 0番目のスキンデフォーマーを取得
		FbxSkin*  fbxSkin = static_cast< FbxSkin* >( pMesh->GetDeformer( 0, FbxDeformer::eSkin ) );

		const  auto  clusterCount = fbxSkin->GetClusterCount();
		if ( clusterCount == 0 )
				return  skin;

		skin.base_inverse.resize( clusterCount );

		const  auto  vtxIndexCount = pMesh->GetPolygonVertexCount();
		const  int*  vtxIndices = pMesh->GetPolygonVertices();

		int  k = 0, j = 0;
		int  indexCounts = 0;
		int*  indices = nullptr;
		double*  weights = nullptr;
		float  w = 0;
		FbxCluster*  currentCluster = nullptr;
		FbxAMatrix  transformMatrix;
		FbxAMatrix  transformLinkMatrix;
		FbxAMatrix  globalBindposeInverseMatrix;

		for ( int i = 0; i < clusterCount; i++ )
		{
				currentCluster = fbxSkin->GetCluster( i );

				assert( currentCluster->GetLinkMode() == FbxCluster::eNormalize );

				std::string  currentJointName = currentCluster->GetLink()->GetName();
				uint  currentJointIndex = FindJointIndexFromName( currentJointName );
				currentCluster->GetTransformMatrix( transformMatrix );
				currentCluster->GetTransformLinkMatrix( transformLinkMatrix );
				globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

				m_skeleton.joints[currentJointIndex].globalBindposeInverse = globalBindposeInverseMatrix;
				m_skeleton.joints[currentJointIndex].pNode = currentCluster->GetLink();

				indexCounts = currentCluster->GetControlPointIndicesCount();
				indices = currentCluster->GetControlPointIndices();
				weights = currentCluster->GetControlPointWeights();
				weightPair.index = currentJointIndex;
				skin.weightPair.resize( pMesh->GetPolygonVertexCount() );
				for ( j = 0; j < indexCounts; j++ )
				{
						weightPair.weight = static_cast< float >( weights[j] );
						skin.weightPair[currentCluster->GetControlPointIndices()[j]] = weightPair;
				}

				//return  skin;
				if ( m_pScene->GetSrcObjectCount<FbxAnimStack>() == 0 )
						continue;

				// アニメーション情報取得
				AnimeContainer  animeContainer;

				FbxAnimStack*  animStack = m_pScene->GetSrcObject < FbxAnimStack>( 0 );
				FbxString  animStackName = animStack->GetName();
				animeContainer.animName = animStackName;

				FbxTakeInfo*  takeInfo = m_pScene->GetTakeInfo( animStackName );
				FbxTime  start = takeInfo->mLocalTimeSpan.GetStart();
				FbxTime  end = takeInfo->mLocalTimeSpan.GetStop();
				animeContainer.animLength = end.GetFrameCount( FbxTime::eFrames24 ) - start.GetFrameCount( FbxTime::eFrames24 ) + 1;
				std::vector<Keyframe>&  keyframes = m_skeleton.joints[currentJointIndex].keyframes;
				keyframes.resize( animeContainer.animLength );
				FbxAMatrix  transformOffset;
				int  keyIndex = 0;

				for ( int64 i = start.GetFrameCount( FbxTime::eFrames24 ); i < end.GetFrameCount( FbxTime::eFrames24 ); i++ )
				{
						FbxTime  time;
						time.SetFrame( i, FbxTime::eFrames24 );
						keyframes[keyIndex].frameNum = i;
						transformOffset = pNode->EvaluateGlobalTransform( time )*geometryTransform;
						keyframes[keyIndex].globalMatrix = transformOffset.Inverse()*currentCluster->GetLink()->EvaluateGlobalTransform( time );
						keyIndex++;
				}

		}// end for

		return  skin;
}


void  FbxLoader::LoadSkeltonHierarchy( FbxNode*  pRoot )
{
		FbxNode*  pNode = nullptr;
		for ( int childIndex = 0; childIndex < pRoot->GetChildCount(); childIndex++ )
		{
				pNode = pRoot->GetChild( childIndex );
				LoadSkeletonHierarchyRecursively( pNode, 0, 0, -1 );
		}
}


void  FbxLoader::LoadSkeletonHierarchyRecursively( FbxNode*  pNode, int  nDepth, int  myIndex, int  parentIndex )
{
		Joint joint;
		if ( pNode->GetNodeAttribute() && pNode->GetNodeAttribute()->GetAttributeType() && pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton )
		{
				joint.parentIndex = parentIndex;
				joint.name = pNode->GetName();
				m_skeleton.joints.push_back( joint );
		}
		for ( int i = 0; i < pNode->GetChildCount(); i++ )
		{
				LoadSkeletonHierarchyRecursively( pNode->GetChild( i ), nDepth + 1, m_skeleton.joints.size(), myIndex );
		}
}


uint  FbxLoader::FindJointIndexFromName( const  std::string&  jointName )
{
		for ( uint i = 0; i < m_skeleton.joints.size(); ++i )
				if ( m_skeleton.joints[i].name == jointName )
						return  i;

		std::cout << "<FbxLoader> Not found joint name :  " << jointName << std::endl;
		return  -1;
}


void  FbxLoader::LoadAnimation( FbxScene*  pScene )
{

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
		for ( auto& container : m_modelContainer )
		{
				container.pMesh = nullptr;
				container.vertices.clear();
				container.indices.clear();
				container.texcoords.clear();
				container.normals.clear();
		}
		m_modelContainer.clear();

		SafeDestroy( m_pScene );
}