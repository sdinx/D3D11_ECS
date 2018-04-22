//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <D3D11Utility\System\IDirect3DRenderer.h>
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Renderable.h>


void  GameUtility::GameLoop()
{
		static  D3D11Utility::IDirect3DRenderer  pd3dRenderer;
		static  BOOL  isInit = true;
		if ( isInit ) 
		{
				D3D11Utility::Camera::SetConstantBuffer();
				D3D11Utility::Renderable::SetConstantBuffer();
				isInit = false;
		}

		pd3dRenderer.Rendering();
}