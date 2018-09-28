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

				//行列へ格納
				btMatrix3x3  btMat;
				btMat.setRotation( origin.GetRotation() );

				// 行列へ格納
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

						/* テンプレート関数 */
						//----------------------------------------------------------------------------------
						//! @func:     AddRigidBody( Transform*,  P&&... ) : void, template<T, P>
						//! @brief:     ワールドに剛体オブジェクトを追加してシェイプを返す.
						//! @param1:  (parent) 剛体オブジェクトに対応させる行列を指定.
						//! @param2:  (mass) 質量.
						//! @param3:  (restitution) 反発係数.
						//! @param4:  (inertia) 慣性モーメント.
						//! @param5:  (param) 剛体形状に合わせてパラメータを指定.
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

								// 剛体オブジェクトの登録
								m_pRigidBodys.push_back( new  btRigidBody( rbInfo ) );
								m_pDynamicsWorld->addRigidBody( m_pRigidBodys.back() );

								return  m_pRigidBodys.back();
						}

				};// class BulletEngine

		}// namespace Systems
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_SYSTEMS_BULLET_ENGINE_