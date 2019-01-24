//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility/Timer.h>
#include  <Engine.h>
#include  <game\Scene\Tutorial.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */


void  Engine::GameInit()
{

}


void  Engine::GameLoop()
{
		static bool isInit = true;
		if ( isInit ) {
				GameScene.SetMethodState( &Scene::BaseScene::Awake );
				GameScene.SetSceneState( Scene::Tutorial::GetInstance() );
				isInit = false;
		}
		GameScene.Update();
}