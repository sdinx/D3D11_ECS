//----------------------------------------------------------------------------------
// file : IPhysical.h
// desc : 物理クラスのインターフェース
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_PHYSICS_2D_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_PHYSICS_2D_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Physical\IPhysics.h>


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  IPhysical :public  Component, public  IPhysics
				{
						friend  class  Systems::PhysicalSystem;

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						IPhysical()
						{}
						virtual  ~IPhysical()
						{}

				private:
						//----------------------------------------------------------------------------------
						// protected variables
						//----------------------------------------------------------------------------------


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
								return  IPhysics::STATIC_COMPONENT_ID;
						}
						static  void  SetStaticComponentId( ComponentId  id )
						{
								if ( IPhysics::STATIC_COMPONENT_ID == STATIC_ID_INVALID )
								{
										IPhysics::STATIC_COMPONENT_ID = id;
										// TODO: need  to output debug string.
								}
						}

						virtual  void  HandleMessage( const  GameUtility::Message&  msg ) = 0;
						virtual  void  HandleMessage( const  GameUtility::Message&  msg, Value  var ) = 0;
						virtual  void  Update() = 0;

				};// class IPhysical

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_PHYSICS_2D_