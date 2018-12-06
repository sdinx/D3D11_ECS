//----------------------------------------------------------------------------------
// File : Mesh.h
// Desc : D3D11Utility::Mesh, a GameObject base class
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_MESH_
#define  _INCLUDED_D3D11_UTILITY_MESH_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility\Component.h>
#include  <d3d11utility\graphics\Texture.h>
#include  <d3d11utility\graphics\VertexBuffer.h>
#include  <d3d11utility\graphics\VertexShader.h>
#include  <d3d11utility\graphics\PixelShader.h>
#include  <d3d11utility\graphics\GeometryShader.h>
#include  <game/GameUtility.h>
#include  <IRenderable.h>
#include  <memory>


namespace  D3D11Utility
{

		class  Renderable : virtual  public  Component, virtual  public  IRenderable
		{
				friend  class  Systems::IDirect3DRenderer;

				struct  ConstantBufferForPerFrame
				{
						Matrix4x4  world;
						Vector4  ambient;
						Vector4  diffuse;
						Vector4  emissive;
						Vector4  specular;
				};

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				Renderable();
				//Renderable( LPCSTR  fileName, Graphics::eRasterMode  rasterMode = Graphics::eFrontSolid );
				Renderable( Graphics::MeshId  meshId, Graphics::eRasterMode  rasterMode = Graphics::eFrontSolid );
				Renderable( Graphics::VertexBuffer*  pVertexBuffer, Graphics::eRasterMode  rasterMode = Graphics::eFrontSolid );
				~Renderable();

				enum  MSG_RENDERABLE
				{
						MSG_UPDATE_CBUFFER,
				};// enum MSG_RENDERABLE

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				static  const  UINT  s_nConstantBufferSlot = eCbufferId::eCBufferRenderable;
				static  ID3D11Buffer  *s_pConstantBuffer;

				std::shared_ptr<Systems::IDirect3DRenderer>  m_pRenderer;
				ConstantBufferForPerFrame  m_cbuffer;
				Graphics::MeshId  m_meshId;
				Graphics::TextureId  m_diffuseId;
				Graphics::TextureId  m_normalId;
				Graphics::VertexBuffer*  m_pVertexBuffer;
				Graphics::VertexShader*  m_pVertexShader;
				Graphics::PixelShader*  m_pPixelShader;
				Graphics::GeometryShader*  m_pGeometryShader;
				Graphics::eRasterMode  m_nRasterMode;

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

				static  ComponentId  GetStaticComponentId()
				{
						return  STATIC_COMPONENT_ID;
				}
				static  void  SetConstantBuffer();
				static  void  SetStaticComponentId( ComponentId  id )
				{
						if ( STATIC_COMPONENT_ID == STATIC_ID_INVALID )
						{
								STATIC_COMPONENT_ID = id;
								// TODO: need  to output debug string.
						}
				}

				void  HandleMessage( const  Message&  msg );
				void  Rendering()const;
				void  Update();
				void  UpdateConstantBuffer( Matrix4x4  world );

				void  SetVertexBuffer( Graphics::VertexBuffer*  pVertexBuffer )
				{
						m_pVertexBuffer = pVertexBuffer;
				}
				void  SetVertexShader( Graphics::VertexShader*  pVertexShader )
				{
						m_pVertexShader = pVertexShader;
				}
				void  SetGeometryShader( Graphics::GeometryShader*  pGeometryShader )
				{
						m_pGeometryShader = pGeometryShader;
				}
				void  SetPixelShader( Graphics::PixelShader*  pPixelShader )
				{
						m_pPixelShader = pPixelShader;
				}
				void  SetRasterMode( Graphics::eRasterMode  rasterMode )
				{
						m_nRasterMode = rasterMode;
				}
				void  SetAmbient( Vector4  color );
				void  SetDiffuse( Vector4  color );
				void  SetEmissive( Vector4  color );
				void  SetSpecular( Vector4  color );
				void  SetDiffuseTexId( Graphics::TextureId  textureId );
				void  SetNormalTexId( Graphics::TextureId  textureId );

				void  Release();
		};

}



#endif // ! _INCLUDED_D3D11_UTILITY_MESH_