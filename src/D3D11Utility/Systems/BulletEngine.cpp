//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\BulletEngine.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <GameUtility.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
SystemId  BulletEngine::STATIC_SYSTEM_ID = STATIC_ID_INVALID;


BulletEngine::BulletEngine()
{
		// 衝突検出方法の設定 ( デフォルト設定 )
		btDefaultCollisionConfiguration*  config = new btDefaultCollisionConfiguration();
		btCollisionDispatcher*  dispatcher = new btCollisionDispatcher( config );

		// ブロードフェーズ法の設定 ( Dynamic AABB tree method )
		btDbvtBroadphase*  broadphase = new btDbvtBroadphase();

		// 剛体間リンクのソルバ設定
		btSequentialImpulseConstraintSolver*  solver = new btSequentialImpulseConstraintSolver();

		// Bullet ワールド作成
		m_pDynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, config );
		m_pDynamicsWorld->setGravity( btVector3( 0, -9.8f, 0 ) );
}


BulletEngine::~BulletEngine()
{
		Release();
}


void  BulletEngine::Update( float  ms )
{
		m_pDynamicsWorld->stepSimulation( 1 / ms, 10 );
}


void  BulletEngine::Release()
{
		for ( auto rb : m_pRigidBodys )
		{
				SafeDelete( rb->getMotionState() );
				m_pDynamicsWorld->removeRigidBody( rb );
				SafeDelete( rb );
		}
		btRigidBody*  pRb = m_pRigidBodys[0];
		m_pRigidBodys.clear();

		SafeDelete( m_pDynamicsWorld->getBroadphase() );
		SafeDelete( m_pDynamicsWorld );
}


void  BulletEngine::AddRigidBody( btRigidBody*  rb )
{
		m_pRigidBodys.push_back( rb );
		m_pDynamicsWorld->addRigidBody( m_pRigidBodys.back() );
}