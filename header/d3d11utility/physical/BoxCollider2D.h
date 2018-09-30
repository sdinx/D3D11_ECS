//----------------------------------------------------------------------------------
// file : BoxCollider2D.h
// desc : 2Dバウンディングボックス
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_BOX_COLLIDER_2D_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_BOX_COLLIDER_2D_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Physical\Collider2D.h>


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  BoxCollider2D :public  Component, virtual  public  Collider2D
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						BoxCollider2D();
						BoxCollider2D( Vector2  center, Vector2  radius );
						virtual  ~BoxCollider2D();

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

				};// class BoxCollider2D

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_BOX_COLLIDER_2D_