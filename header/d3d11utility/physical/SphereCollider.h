//----------------------------------------------------------------------------------
// file : SphereCollider.h
// desc : 3Dバウンディングスフィア
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_SPHERE_COLLIDER_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_SPHERE_COLLIDER_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Physical\Collider.h>


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  SphereCollider :public  Component, virtual  public  Collider
				{
						friend  class  Collider;

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						SphereCollider();
						SphereCollider( Vector3  center, Vector3  radius );
						virtual  ~SphereCollider();

				private:
						//----------------------------------------------------------------------------------
						// protected variables
						//----------------------------------------------------------------------------------

						static  ComponentId  STATIC_COMPONENT_ID;

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
						void  HandleMessage( const  GameUtility::Message&  msg )
						{}
						void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
						{}
						void  Update();

				};// class SphereCollider

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_SPHERE_COLLIDER_