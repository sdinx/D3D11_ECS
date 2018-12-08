//----------------------------------------------------------------------------------
// file : Material.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_GRAPHICS_MATERIAL_
#define  _INCLUDED_D3D11_UTILITY_GRAPHICS_MATERIAL_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility/Interface.h>


namespace  D3D11Utility
{

		namespace  Graphics
		{

				class  Material
				{
						struct  CBufferMaterial
						{
								Vector3  g_ambient;
								Vector3  g_diffuse;
								Vector4  g_emissive;
								Vector4  g_specular;
						};// struct CBufferMaterial

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------
						Material();
						Material( const  Material&  mat );
						Material( const  MaterialId  _id );
						Material( Vector3  _ambient, Vector3  _diffuse, Vector4  _specular, Vector4  _emissive );
						Material( const  MaterialId  _id, Vector3  _ambient, Vector3  _diffuse, Vector4  _specular, Vector4  _emissive );
						~Material();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						static  const  UINT  s_nConstantBufferSlot = eCbufferId::eCBufferMaterial;
						static  ID3D11Buffer  *s_pConstantBuffer;

						const  MaterialId  materialId;
						union
						{
								CBufferMaterial  m_cbuffer;
						};
						Vector3  ambient;
						Vector3  diffuse;
						Vector4  specular;
						Vector4  emissive;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------


				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------


				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						/* static methods */
						static  void  SetConstantBuffer();

						/* Getter */
						MaterialId  GetMaterialId()
						{
								return  materialId;
						}
						Vector3  GetAmbient()
						{
								return  ambient;
						}
						Vector3  GetDiffuse()
						{
								return  diffuse;
						}
						Vector4  GetSpecular()
						{
								return  specular;
						}
						Vector4  GetEmissive()
						{
								return  emissive;
						}

						/* Setter */
						void  SetAmbient( Vector3  v3Color )
						{
								ambient = v3Color;
						}
						void  SetDiffuse( Vector3  v3Color )
						{
								diffuse = v3Color;
						}
						void  SetSpecular( Vector4  v4Color )
						{
								specular = v4Color;
						}
						void  SetEmissive( Vector4  v4Color )
						{
								emissive = v4Color;
						}

						/* Utilities */
						void  UpdateConstantBuffer();
						void  Release();

				}; // class Material

		}// namespace Graphics

}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GRAPHICS_MATERIAL_