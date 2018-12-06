//----------------------------------------------------------------------------------
// file: FbxLoader.h
// desc: FBXファイルの読み込みに関するローダークラス
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11UTILITY_SYSTEMS_FBX_LOADER_
#define  _INCLUDED_D3D11UTILITY_SYSTEMS_FBX_LOADER_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <fbxsdk.h>
#include  <string>
#include  <vector>
#include  <memory>


namespace  D3D11Utility
{
		struct  SkinMesh
		{
				std::vector<std::vector<float>>  weights;
				std::vector<FbxAMatrix>  base_inverse;
		};// struct SkinMesh

		struct  AnimeContainer
		{
				FbxString  animName;
				int64  animLength;
		};// struct AnimeContainer

		struct  ModelContainer
		{
				~ModelContainer()
				{
						Release();
				}

				FbxMesh*  pMesh = nullptr;
				std::vector<Vector3>  vertices;
				std::vector<Vector3>  normals;
				std::vector<Vector2>  texcoords;
				std::vector<INT>  indices;
				SkinMesh  skinMeshes;
				void  Release()
				{
						vertices.clear();
						normals.clear();
						texcoords.clear();
						indices.clear();
				}
		};// struct ModelContainer

		namespace  Systems
		{

				class  FbxLoader
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						FbxLoader();
						FbxLoader( FbxString  szFileName );
						~FbxLoader();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						static  FbxManager*  s_pFbxManager;

						FbxScene*  m_pScene;
						FbxImporter*  m_pImporter;
						std::vector<ModelContainer>  m_modelContainer;
						std::vector<Material>  m_materials;
						std::vector<AnimeContainer>  m_animeContainer;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------

						ModelContainer  LoadMesh( FbxScene*  pScene );
						std::vector<Vector3>  LoadVertices( FbxMesh*  pMesh );
						Vector3  LoadNormal( FbxGeometryElementNormal*&  pNormals, uint  vertexIndex, uint  vertexCount );
						Vector2  LoadTexcoord( FbxGeometryElementUV*&  pTexcoords, uint  vertexIndex, uint  texcoordIndex );
						std::vector<INT>  LoadIndices( FbxMesh*  pMesh );
						Material  LoadMaterial( FbxSurfaceMaterial*  material );
						SkinMesh  LoadSkin( FbxMesh*  pMesh );

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						static  void  CreateFbxManager();
						static  FbxManager*  GetFbxManager()
						{
								return  s_pFbxManager;
						}
						static  void  StaticRelease()
						{
								s_pFbxManager->Destroy();
						}

						void  LoadFbxModel( FbxScene*  pScene );
						uint32  GetModelCounts()const
						{
								return  m_modelContainer.size();
						}
						ModelContainer  GetModelContainer( uint  i )const
						{
								if ( m_modelContainer.size() > i )
										return  m_modelContainer[i];
								return  ModelContainer();
						}
						Material  GetMaterial( uint  i )const
						{
								if ( m_materials.size() > i )
										return  m_materials[i];
								return  Material();
						}
						Matrix4x4  GetMeshMatrix( uint  frame, uint  meshIndex );
						ModelContainer  SetAnimation( uint  meshIndex, uint  animationIndex, FbxTime&  fbxTime );
						void  SetBoneMatrix( uint  frame, uint  meshIndex );
						void  Release();

				};// class FbxLoader

		}// namespace Systems
}// namespace D3D11Utiltiy


#endif // ! _INCLUDED_D3D11UTILITY_SYSTEMS_FBX_LOADER_