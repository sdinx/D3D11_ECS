//----------------------------------------------------------------------------------
// file : Physical.h
// desc : 3éüå≥ãÛä‘Ç…Ç®ÇØÇÈï®óùÉNÉâÉX
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_PHYSICS_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_PHYSICS_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Physical\IPhysics.h>


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  Physics :public  Component, public  IPhysics
				{
						friend  class  Systems::PhysicalSystem;

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						Physics()
						{}
						virtual  ~Physics()
						{}

				private:
						//----------------------------------------------------------------------------------
						// private variables
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

				};// class Physics

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_PHYSICS_