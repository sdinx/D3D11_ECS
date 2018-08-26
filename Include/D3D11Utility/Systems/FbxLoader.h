//----------------------------------------------------------------------------------
// file: FbxLoader.h
// desc: FBXファイルの読み込みに関するローダークラス
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11UTILITY_SYSTEMS_FBX_LOADER_
#define  _INCLUDED_D3D11UTILITY_SYSTEMS_FBX_LOADER_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
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
		};

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

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						static  void  CreateFbxManager();
						static  FbxManager*  GetFbxManager()
						{
								return  s_pFbxManager;
						}

						ModelContainer  GetModelContainer( UINT  i )
						{
								if ( m_modelContainer.size() > i )
										return  m_modelContainer[i];
								return  ModelContainer();
						}
						Material  GetMaterial( UINT  i )
						{
								if ( m_materials.size() > i )
										return  m_materials[i];
								return  Material();
						}
						std::vector<Vector3>  LoadVertices( FbxMesh*  pMesh, INT  size );
						std::vector<Vector3>  LoadNormals( FbxMesh*  pMesh, INT  size );
						std::vector<Vector2>  LoadTexcoords( FbxMesh*  pMesh, INT  size );
						std::vector<INT>  LoadIndices( FbxMesh*  pMesh );
						Material  LoadMaterial( FbxSurfaceMaterial*  material );
						SkinMesh  LoadSkin( FbxMesh*  pMesh );
						ModelContainer  LoadMesh( FbxScene*  pScene );
						void  Release();

				};// class FbxLoader

		}// namespace Systems
}// namespace D3D11Utiltiy


#endif // ! _INCLUDED_D3D11UTILITY_SYSTEMS_FBX_LOADER_