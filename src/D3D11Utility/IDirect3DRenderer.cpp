//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <IDirect3DRenderer.h>

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


		}
		m_pID3D->EndRender();
}