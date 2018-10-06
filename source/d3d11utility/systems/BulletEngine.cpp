//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\Entity.h>
#include  <d3d11utility\systems\BulletEngine.h>
#include  <d3d11utility\systems\ComponentManager.h>
#include  <d3d11utility\systems\EntityManager.h>
#include  <d3d11utility\physical\BulletPhysics.h>
#include  <game/GameUtility.h>


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
		btTransform  btTrans;
		for ( auto rb : m_pComponentManager->GetComponents<Physical::BulletPhysics>() )
		{
				trans = rb->GetComponent<Transform>();
				// �����v�Z��̃��[���h�s����擾
				btTrans = rb->GetComponent<Physical::BulletPhysics>()->GetRigidBody()->getWorldTransform();

				// �����v�Z��̈ʒu����ݒ�
				trans->SetPosition( btTrans.getOrigin() );
				trans->SetRotation( btTrans.getRotation() );
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