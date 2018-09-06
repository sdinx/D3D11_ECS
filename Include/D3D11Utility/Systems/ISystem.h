//----------------------------------------------------------------------------------
// file : ISystem.h
// desc : システムのインターフェース
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_INTERFACE_SYSTEM_
#define  _INCLUDED_D3D11_UTILITY_INTERFACE_SYSTEM_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\Interface.h>

namespace  D3D11Utility
{
		namespace  Systems
		{
				using  SystemId = int;
				static  const  SystemId  SYSTEM_ID_INVALID = STATIC_ID_INVALID;

				class  ISystem
				{
						friend  class  SystemManager;

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						ISystem() :
								m_isActive( true )
						{}
						virtual  ~ISystem()
						{}

				protected:
						//----------------------------------------------------------------------------------
						// protected variables
						//----------------------------------------------------------------------------------

						BOOL  m_isActive;
						ComponentManager*  m_pComponentManager;
						EntityManager*  m_pEntityManager;

				public:
						//----------------------------------------------------------------------------------
						// public variables
						//----------------------------------------------------------------------------------
						/* NOTHING */

				protected:
						//----------------------------------------------------------------------------------
						// protected methods
						//----------------------------------------------------------------------------------
						/* NOTHING */

				public:
						//----------------------------------------------------------------------------------
						// public methods
						//----------------------------------------------------------------------------------

						virtual  SystemId  GetSystemId()const = 0;
						volatile  void  SetActive( BOOL  isActive )
						{
								m_isActive = isActive;
						}
						virtual  void  Update( float  ms ) = 0;
						virtual  void  Release() = 0;

				};// class ISystem

		}// namespace Systems
}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_INTERFACE_SYSTEM_