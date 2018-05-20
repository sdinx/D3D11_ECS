//----------------------------------------------------------------------------------
// file : PhysicalSystem.h
// desc : 物理オブジェクトの管理クラス
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_SYSTEMS_PHYSICAL_SYSTEM_
#define  _INCLUDED_D3D11_UTILITY_SYSTEMS_PHYSICAL_SYSTEM_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility/Systems/ISystem.h>
#include  <D3D11Utility\Physical\IPhysics.h>


namespace  D3D11Utility
{
		namespace  Systems
		{

				class  PhysicalSystem :public  ISystem
				{
				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						PhysicalSystem()
						{}
						virtual  ~PhysicalSystem()
						{}

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
						void  Update( float  ms );

				};// class PhysicalSystem

		}// namespace Systems
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_SYSTEMS_PHYSICAL_SYSTEM_