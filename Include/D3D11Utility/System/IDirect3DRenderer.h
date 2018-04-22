//----------------------------------------------------------------------------------
// file : IDirect3DRenderer.h
// desc : 
//----------------------------------------------------------------------------------


#ifndef  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_
#define  _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\System\IDirect3D.h>
#include  <D3D11Utility\System\RenderManager.h>
#include  <IGraphicsRenderer.h>
#include  <IRenderable.h>
#include  <Vector>


namespace  D3D11Utility
{

		class  IDirect3DRenderer :public  IGraphicsRenderer
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				IDirect3DRenderer();
				~IDirect3DRenderer();


		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------
				std::shared_ptr<IDirect3D>  m_pID3D;
				std::vector<IRenderable*>  m_pRenderObjects;


		public:
				//----------------------------------------------------------------------------------
				// public  variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private  functions
				//----------------------------------------------------------------------------------
				/* NOTHING */

		public:
				//----------------------------------------------------------------------------------
				// public  functions
				//----------------------------------------------------------------------------------
				VOID  Release();
				VOID  Rendering()const;
		};

}

#endif // ! _INCLUDED_INTERFACE_D3D11_UTILITY_DIRECT3D_RENDERER_