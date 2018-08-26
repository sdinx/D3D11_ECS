//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <GameUtility.h>
#include  <Game\Scene\Tutorial.h>


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