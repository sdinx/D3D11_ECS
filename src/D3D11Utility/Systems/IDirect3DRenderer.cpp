//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\IDirect3DRenderer.h>
#include  <GameUtility.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
SystemId  IDirect3DRenderer::STATIC_SYSTEM_ID = STATIC_ID_INVALID;


IDirect3DRenderer::IDirect3DRenderer() :
		m_pID3D( _Singleton<IDirect3D>::GetInstance() )
{

}


IDirect3DRenderer::~IDirect3DRenderer()
{
		Release();
}


VOID  IDirect3DRenderer::Release()
{

}


VOID  IDirect3DRenderer::Rendering()const
{
		m_pID3D->BeginRender();
		{/* Begin rendering */
				static  Renderable  mesh( PT_CUBE );
				static  bool  init = true;
				if ( init ) {
						init = false;
				}
				mesh.Rendering();

				for ( auto renderable : m_renderObjects )
						renderable->Rendering();

		}/* Done rendering */
		m_pID3D->EndRender();

}// end Rendering()const