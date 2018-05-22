//----------------------------------------------------------------------------------
// file : IDirect3DRenderer.h
// desc : 
//----------------------------------------------------------------------------------


#ifndef  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_
#define  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Interface.h>
#include  <D3D11Utility\Systems\IDirect3D.h>
#include  <IGraphicsRenderer.h>
#include  <IRenderable.h>
#include  <D3D11Utility\Graphics\VertexShader.h>
#include  <D3D11Utility\Graphics\GeometryShader.h>
#include  <D3D11Utility\Graphics\PixelShader.h>


namespace  D3D11Utility
{
		namespace  Systems
		{

				class  IDirect3DRenderer :public  IGraphicsRenderer
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						IDirect3DRenderer( ComponentManager*  pComponentManagerInstance );
						~IDirect3DRenderer();


				private:
						//----------------------------------------------------------------------------------
						// private  variables
						//----------------------------------------------------------------------------------

						std::shared_ptr<IDirect3D>  m_pID3D;
						ComponentManager*  m_componentManager;
						std::vector<Graphics::VertexShader>  m_vertexShaderList;
						std::vector<Graphics::GeometryShader>  m_geometryShaderList;
						std::vector<Graphics::PixelShader>  m_pixelShaderList;

				public:
						//----------------------------------------------------------------------------------
						// public  variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private  methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public  methods
						//----------------------------------------------------------------------------------

						Graphics::ShaderId  CreateShaderFromFile( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szShaderModel );
						void  Release();
						void  Rendering()const;

				};// class IDirect3DRenderer

		}// namespace Systems
}// namespace D3D11Utility

#endif // ! _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_