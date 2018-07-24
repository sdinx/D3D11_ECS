//----------------------------------------------------------------------------------
// file : IPhysics.h
// desc : 物理クラスのインターフェース
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_INTERFACE_PHYSICS_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_INTERFACE_PHYSICS_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\IComponent.h>
#include  <D3D11Utility\Interface.h>


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  IPhysics
				{
						friend  class  Systems::PhysicalSystem;

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						IPhysics()
						{}
						virtual  ~IPhysics()
						{}

				protected:
						//----------------------------------------------------------------------------------
						// protected variables
						//----------------------------------------------------------------------------------

						static  ComponentId  STATIC_COMPONENT_ID;

						bool  m_isGravity;
						float  m_velocity;
						float  m_gravity;
						float  m_time;
						float  m_mass;

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

				};// class IPhysics

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_INTERFACE_PHYSICS_