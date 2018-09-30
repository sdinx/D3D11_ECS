//----------------------------------------------------------------------------------
// file : BulletPhysics.h
// desc : Bullet Physics を使った物理コンポーネント.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_BULLET_PHYSICS_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_BULLET_PHYSICS_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Interface.h>

//----------------------------------------------------------------------------------
// pragmas
//----------------------------------------------------------------------------------
#if  defined(DEBUG)||(_DEBUG)
#pragma  comment(lib, "BulletCollision_vs2010_debug.lib")
#pragma  comment(lib, "BulletDynamics_vs2010_debug.lib")
#pragma  comment(lib, "LinearMath_vs2010_debug.lib")
#else
#pragma  comment(lib, "BulletCollision_vs2010.lib")
#pragma  comment(lib, "BulletDynamics_vs2010.lib")
#pragma  comment(lib, "LinearMath_vs2010.lib")
#endif


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  BulletPhysics :public  Component
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						BulletPhysics( btRigidBody*  pRigidBody, const  Systems::BulletEngine* );
						virtual  ~BulletPhysics();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						static  ComponentId  STATIC_COMPONENT_ID;

						const  Systems::BulletEngine*  m_pBulletEngine;
						btRigidBody*  m_pRigidBody;

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

						static  ComponentId  GetStaticComponentId()
						{
								return  STATIC_COMPONENT_ID;
						}
						static  void  SetStaticComponentId( ComponentId  id )
						{
								if ( STATIC_COMPONENT_ID == STATIC_ID_INVALID )
								{
										STATIC_COMPONENT_ID = id;
										// TODO: need  to output debug string.
								}
						}

						void  HandleMessage( const  Message&  msg );
						void  Update();
						void  Release();
						btRigidBody*  GetRigidBody()
						{
								return  m_pRigidBody;
						}

				};// class BulletPhysics

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_BULLET_PHYSICS_