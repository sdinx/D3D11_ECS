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


IDirect3DRenderer::IDirect3DRenderer( ComponentManager*  pComponentManagerInstance ) :
		m_componentManager( pComponentManagerInstance ),
		m_pID3D( _Singleton<IDirect3D>::GetInstance() )
{

}


IDirect3DRenderer::~IDirect3DRenderer()
{
		Release();
}


void  IDirect3DRenderer::Release()
{

}


void  IDirect3DRenderer::Rendering()const
{
		m_pID3D->BeginRender();
		{/* Begin rendering */

				for ( auto renderable : m_componentManager->GetComponents<Renderable>() )
				{
						renderable->Update();
						renderable->Rendering();
				}// end for

		}/* Done rendering */
		m_pID3D->EndRender();

}// end Rendering()const


Graphics::ShaderId  IDirect3DRenderer::CreateShaderFromFile( LPCWSTR  szFileName, LPCSTR  szEntryPoint, LPCSTR  szShaderModel )
{
		return  0;
}