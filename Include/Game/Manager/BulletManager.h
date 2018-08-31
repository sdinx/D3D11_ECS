//----------------------------------------------------------------------------------
// file: BulletManager.h
// desc: 
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_SYSTEM_BULLET_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_SYSTEM_BULLET_MANAGER_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility/Systems/ISystem.h>
#include  <D3D11Utility\Transform.h>


namespace  D3D11Utility
{
		namespace  Systems
		{

				class  BulletManager :public  ISystem
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						BulletManager( const  Entity*  bulletOriginal );
						~BulletManager();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						static  SystemId  STATIC_SYSTEM_ID;

						const  Entity*  m_bulletOriginal;

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
						void  CreateBullet( const  Transform  spawnTransform, const  float  fBulletSpeed );
						void  CreateBullet( const  Vector3  spawnPosition,  const  Vector3  spawnRotate, const  float  fBulletSpeed );

				};// class BulletManager

		}// namespace Systems
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_SYSTEM_BULLET_MANAGER_