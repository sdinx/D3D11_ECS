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
#include  <D3D11Utility\Graphics\VertexBuffer.h>
#include  <D3D11Utility\Graphics\VertexShader.h>
#include  <D3D11Utility\Graphics\PixelShader.h>
#include  <D3D11Utility\Graphics\GeometryShader.h>
#include  <GameUtility.h>
#include  <IRenderable.h>
#include  <memory>

//----------------------------------------------------------------------------------
// namespace D3D11UItility class
//----------------------------------------------------------------------------------
namespace  D3D11Utility
{

		class  Renderable : public  Component, public  IRenderable
		{
				// TODO: É|ÉäÉSÉìèoÇ»Ç¢ïa
		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Renderable();
				Renderable( PRIMITIVE_TYPE  primitiveType );
				~Renderable()
				{}


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				static  std::unique_ptr<CONSTANTBUFFER>  s_pCBuffer;

				std::unique_ptr<Graphics::VertexBuffer>  m_pVertexBuffer;
				std::unique_ptr<Graphics::VertexShader>  m_pVertexShader;
				std::unique_ptr<Graphics::PixelShader>  m_pPixelShader;
				std::unique_ptr<Graphics::GeometryShader>  m_pGeometryShader;


		protected:
				//----------------------------------------------------------------------------------
				// protected variables
				//----------------------------------------------------------------------------------
				Matrix4x4  m_localWorld;


		private:
				//----------------------------------------------------------------------------------
				// private methods
				//----------------------------------------------------------------------------------



		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------
				static  void  SetConstantBuffer();

				void  HandleMessage( const  GameUtility::Message&  msg )
				{}
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
				{}
				void  Rendering()const;
				void  Update()
				{}


		};

}



#endif // ! _INCLUDED_D3D11_UTILITY_MESH_