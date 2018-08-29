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

						BulletManager();
						~BulletManager();

				private:
						//----------------------------------------------------------------------------------
						// private variables
						//----------------------------------------------------------------------------------

						static  SystemId  STATIC_SYSTEM_ID;

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

						SystemId  GetSystemId()const
						{
								return  STATIC_SYSTEM_ID;
						}
						void  Update( FLOAT  ms );
						void  Release();

				};// class BulletManager

		}// namespace Systems
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_SYSTEM_BULLET_MANAGER_