//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>
#include  <D3D11Utility\System\IDirect3DRenderer.h>
#include  <GameUtility.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;


IDirect3DRenderer::IDirect3DRenderer()
{
		m_pID3D = _Singleton<IDirect3D>::GetInstance();
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
		{
				static  Camera  camera;
				static  Renderable  mesh( PT_CUBE );
				static  bool  init = true;
				if ( init ) {
 						camera.SetPosition( Vector3( 0.0f, 0.0f, -0.75f ) );
						camera.SetTarget( Vector3( 0.0f, 0.0f, 0.0f ) );
						camera.HandleMessage( GameUtility::Message( MSG_UPDATE_ALL ) );
						init = false;
				}
				mesh.Rendering();

		}
		m_pID3D->EndRender();
}// end Rendering()const