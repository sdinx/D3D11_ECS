//----------------------------------------------------------------------------------
// file: BulletEngine.h
// desc: 
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_SYSTEMS_BULLET_ENGINE_
#define  _INCLUDED_D3D11_UTILITY_SYSTEMS_BULLET_ENGINE_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include  <D3D11Utility/Systems/ISystem.h>
#include  <D3D11Utility/Transform.h>
#include  <vector>


namespace  D3D11Utility
{
		/* (dxmath to bullet) or (bullet to dxmath) */
		inline  btTransform  BulletConvertTransform( Transform  origin )
		{
				btTransform  btOut;
				const  Matrix4x4  mat = origin.GetWorld();

				//�s��֊i�[
				btMatrix3x3  btMat;
				btMat.setRotation( origin.GetRotation() );

				// �s��֊i�[
				btOut.setRotation( origin.GetRotation() );
				btOut.setOrigin( origin.GetPosition() );

				return  btOut;
		}

		namespace  Systems
		{

				class  BulletEngine :public  ISystem
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						BulletEngine();
						~BulletEngine();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						static  SystemId  STATIC_SYSTEM_ID;

						btDynamicsWorld*  m_pDynamicsWorld;
						std::vector<btRigidBody*>  m_pRigidBodys;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				private:
						//----------------------------------------------------------------------------------
						// private methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						static  SystemId  GetStaticSystemId()
						{
								return  STATIC_SYSTEM_ID;
						}
						static  void  SetStaticSystemId( SystemId  systemId )
						{
								if ( STATIC_SYSTEM_ID == STATIC_ID_INVALID )
										STATIC_SYSTEM_ID = systemId;
						}

						/* virtual */
						SystemId  GetSystemId()const
						{
								return  STATIC_SYSTEM_ID;
						}
						void  Update( FLOAT  ms );
						void  Release();

						/* original */
						void  AddRigidBody( btRigidBody*  rb );

						/* �e���v���[�g�֐� */
						//----------------------------------------------------------------------------------
						//! @func:     AddRigidBody( Transform*,  P&&... ) : void, template<T, P>
						//! @brief:     ���[���h�ɍ��̃I�u�W�F�N�g��ǉ����ăV�F�C�v��Ԃ�.
						//! @param1:  (parent) ���̃I�u�W�F�N�g�ɑΉ�������s����w��.
						//! @param2:  (mass) ����.
						//! @param3:  (restitution) �����W��.
						//! @param4:  (inertia) �������[�����g.
						//! @param5:  (param) ���̌`��ɍ��킹�ăp�����[�^���w��.
						//----------------------------------------------------------------------------------
						template<class  T, typename  ...P>
						btRigidBody*  CreateRigidBody( Transform*  parent, btScalar  mass, btScalar  restitution, btVector3  inertia, P&&... param )
						{
								parent->Update();

								btTransform  btTrans = BulletConvertTransform( *parent );
								btCollisionShape*  pCollisionShape = new  T( std::forward<P>( param )... );
								pCollisionShape->calculateLocalInertia( mass, inertia );

								btDefaultMotionState*  motionState = new  btDefaultMotionState( btTrans );
								btRigidBody::btRigidBodyConstructionInfo  rbInfo( mass, motionState, pCollisionShape, inertia );

								// ���̃I�u�W�F�N�g�̓o�^
								m_pRigidBodys.push_back( new  btRigidBody( rbInfo ) );
								m_pDynamicsWorld->addRigidBody( m_pRigidBodys.back() );

								return  m_pRigidBodys.back();
						}

				};// class BulletEngine

		}// namespace Systems
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_SYSTEMS_BULLET_ENGINE_