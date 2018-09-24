//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\BulletEngine.h>
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Systems\EntityManager.h>
#include  <D3D11Utility\Physical\BulletPhysics.h>
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
		// �Փˌ��o���@�̐ݒ� ( �f�t�H���g�ݒ� )
		btDefaultCollisionConfiguration*  config = new btDefaultCollisionConfiguration();
		btCollisionDispatcher*  dispatcher = new btCollisionDispatcher( config );

		// �u���[�h�t�F�[�Y�@�̐ݒ� ( Dynamic AABB tree method )
		btDbvtBroadphase*  broadphase = new btDbvtBroadphase();

		// ���̊ԃ����N�̃\���o�ݒ�
		btSequentialImpulseConstraintSolver*  solver = new btSequentialImpulseConstraintSolver();

		// Bullet ���[���h�쐬
		m_pDynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, config );
		m_pDynamicsWorld->setGravity( btVector3( 0, -9.8f, 0 ) );
}


BulletEngine::~BulletEngine()
{
		Release();
}


void  BulletEngine::Update( float  ms )
{
		m_pDynamicsWorld->stepSimulation( 1 / 600.0f, 10 );

		Transform*  trans = nullptr;
		Vector3  v;
		Physical::BulletPhysics*  physics = nullptr;
		btTransform  btTrans;
		for ( auto rb : m_pComponentManager->GetComponents<Physical::BulletPhysics>() )
		{
				trans = rb->GetComponent<Transform>();
				physics = rb->GetComponent<Physical::BulletPhysics>();
				btTrans = physics->GetRigidBody()->getCenterOfMassTransform();
				v = btTrans.getOrigin();
				v -= trans->GetLocalPosition();
				trans->SetPosition( v );
		}
}


void  BulletEngine::Release()
{
		for ( auto rb : m_pRigidBodys )
		{
				SafeDelete( rb->getMotionState() );
				m_pDynamicsWorld->removeRigidBody( rb );
				SafeDelete( rb );
		}
		m_pRigidBodys.clear();

		SafeDelete( m_pDynamicsWorld->getBroadphase() );
		SafeDelete( m_pDynamicsWorld );
}


void  BulletEngine::AddRigidBody( btRigidBody*  rb )
{
		m_pRigidBodys.push_back( rb );
		m_pDynamicsWorld->addRigidBody( m_pRigidBodys.back() );
}