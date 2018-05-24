//----------------------------------------------------------------------------------
// File : Mesh.h
// Desc : D3D11Utility::Mesh, a GameObject base class
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_MESH_
#define  _INCLUDED_D3D11_UTILITY_MESH_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Graphics\Texture.h>
#include  <D3D11Utility\Graphics\VertexBuffer.h>
#include  <D3D11Utility\Graphics\VertexShader.h>
#include  <D3D11Utility\Graphics\PixelShader.h>
#include  <D3D11Utility\Graphics\GeometryShader.h>
#include  <D3D11Utility\Graphics\IShader.h>
#include  <GameUtility.h>
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
						Vector4  meshColor;
				};

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				Renderable();
				Renderable( PRIMITIVE_TYPE  primitiveType );
				Renderable( LPCSTR  fbxString );
				~Renderable()
				{}

				enum  MSG_RENDERABLE
				{
						MSG_UPDATE_CBUFFER,
				};// enum MSG_RENDERABLE

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				static  const  UINT  s_nConstantBufferSlot = 1;
				static  ID3D11Buffer  *s_pConstantBuffer;

				ConstantBufferForPerFrame  m_cbuffer;
				Graphics::TextureId  m_textureId = TEXTURE_ID_INVALID;
				Graphics::VertexBuffer*  m_pVertexBuffer = nullptr;
				Graphics::VertexShader*  m_pVertexShader = nullptr;
				Graphics::PixelShader*  m_pPixelShader = nullptr;
				Graphics::GeometryShader*  m_pGeometryShader = nullptr;
				Systems::TextureManager*  m_textureManager = nullptr;

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

				static  void  SetConstantBuffer();
				static  ComponentId  GetStaticComponentId()
				{
						return  STATIC_COMPONENT_ID;
				}
				static  void  SetStaticComponentId( ComponentId  id )
				{
						if ( STATIC_COMPONENT_ID == STATIC_ID_INVALID )
						{
								STATIC_COMPONENT_ID = id;
								// TODO: need  to output debug string.
						}
				}

				void  HandleMessage( const  GameUtility::Message&  msg );
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
				{}
				void  Rendering()const;
				void  Update();
				void  UpdateConstantBuffer( Matrix4x4  world );
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
				void  SetColor( Vector4  v4Color );
				void  SetTextureId( Graphics::TextureId  textureId,  Systems::TextureManager*  textureManagerInstance = nullptr );
		};

}



#endif // ! _INCLUDED_D3D11_UTILITY_MESH_