//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <IDirect3DRenderer.h>


void  GameUtility::GameLoop()
{
		static  D3D11Utility::IDirect3DRenderer  pd3dRenderer;
		static  BOOL  isInit = TRUE;
		if ( isInit ) 
		{
				D3D11Utility::Mesh::SetConstantBuffer();
				D3D11Utility::Camera::SetConstantBuffer();
				D3D11Utility::Scene  scene;
				D3D11Utility::Camera  camera;
				camera.SetPosition( D3D11Utility::Vector3( 0, 1.0f, 0 ) );
				camera.SetTarget( D3D11Utility::Vector3( 0, 0.0f, 0 ) );

				scene.CreateCamera( camera );
				scene.CreateGameObject();
				pd3dRenderer.AddScene( scene );
				pd3dRenderer.AddRenderMesh( &scene );
				isInit = FALSE;
		}

		pd3dRenderer.Rendering();
}