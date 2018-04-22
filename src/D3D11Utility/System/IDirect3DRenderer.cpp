//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\System\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>

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
						camera.SetPosition( Vector3( 0.0f, 0.0f, -50.0f ) );
						camera.SetTarget( Vector3( 0.0f, 0.0f, 5.0f ) );
						camera.UpdateView();
				}
				camera.Update();
				mesh.Rendering();

		}
		m_pID3D->EndRender();
}