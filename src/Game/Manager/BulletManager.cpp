//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
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


BulletManager::BulletManager( const  Entity*  bulletOriginal ) :
		m_bulletOriginal( bulletOriginal )
{

}


BulletManager::~BulletManager()
{

}


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


void  BulletManager::CreateBullet( const  Transform  spawnTransform, const  float  fBulletSpeed )
{
		const  EntityId  bulletId = m_pEntityManager->CreateEntity( "bullet" );
		Entity*  bullet = m_pEntityManager->GetEntity( bulletId );


}


void  BulletManager::CreateBullet( const  Vector3  spawnPosition, const  Vector3  spawnRotate, const  float  fBulletSpeed )
{

}