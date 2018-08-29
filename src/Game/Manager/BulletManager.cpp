//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <GameUtility.h>
#include  <Game\Bullet.h>
#include  <Game/Manager/BulletManager.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
SystemId  BulletManager::STATIC_SYSTEM_ID = STATIC_ID_INVALID;


void  BulletManager::Update( float  ms )
{
		for ( auto bullet : m_pComponentManager->GetComponents<Bullet>() )
		{
				bullet->Update();
		}
}


void  BulletManager::Release()
{

}