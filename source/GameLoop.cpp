//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <game/GameUtility.h>
#include  <game\Scene\Tutorial.h>
#include  <d3d11utility/Timer.h>
using  namespace  D3D11Utility::Systems;


void  GameUtility::GameInit()
{

}


void  GameUtility::GameLoop()
{
		static bool isInit = true;
		if ( isInit ) {
				GameScene.SetMethodState( &Scene::BaseScene::Awake );
				GameScene.SetSceneState( Scene::Tutorial::GetInstance() );
				isInit = false;
		}
		GameScene.Update();
}