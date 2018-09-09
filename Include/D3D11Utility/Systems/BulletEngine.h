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
				const  Matrix4x4  mat = origin.GetWorldMatrix();

				btMatrix3x3  btMat;
				btMat.setValue(
						mat._11, mat._12, mat._13,
						mat._21, mat._22, mat._23,
						mat._31, mat._32, mat._33
				);

				// 行列へ格納
				btOut.setBasis( btMat );
				btOut.setOrigin( btVector3( mat._41, mat._42, mat._43 ) );

				return  btOut;
		}
		inline  Matrix4x4  BulletConvertMatrix4x4( btTransform  btOrigin )
		{
				btMatrix3x3  btMat = btOrigin.getBasis();
				btVector3  v = btOrigin.getOrigin();

				Matrix4x4  out;
				{
						out._11 = btMat[0].m_floats[0];
						out._12 = btMat[0].m_floats[1];
						out._13 = btMat[0].m_floats[2];
						out._14 = 0.0f;

						out._21 = btMat[1].m_floats[0];
						out._22 = btMat[1].m_floats[1];
						out._23 = btMat[1].m_floats[2];
						out._24 = 0.0f;

						out._31 = btMat[2].m_floats[0];
						out._32 = btMat[2].m_floats[1];
						out._33 = btMat[2].m_floats[2];
						out._34 = 0.0f;

						out._41 = v.m_floats[0];
						out._42 = v.m_floats[1];
						out._43 = v.m_floats[2];
						out._44 = 0.0f;
				}

				return  out;
		}
		inline  btTransform  BulletMultiply( const  btTransform&  btOrigin, const  Transform&  trans )
		{
				btTransform  btOut = btOrigin;
				btTransform  btTrans = BulletConvertTransform( trans );

				btOut *= btTrans;

				return  btOut;
		}
		inline  Matrix4x4  BulletMultiply( const  Matrix4x4&  origin, const  btTransform&  btTrans )
		{
				Matrix4x4  out = origin;
				Matrix4x4  mat = BulletConvertMatrix4x4( btTrans );

				DirectX::XMStoreFloat4x4( 
						&out, DirectX::XMMatrixMultiply( 
								DirectX::XMLoadFloat4x4( &out ), DirectX::XMLoadFloat4x4( &mat ) ) );

				return  out;
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