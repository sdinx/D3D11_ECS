//----------------------------------------------------------------------------------
// file : BoxCollider.h
// desc : 3Dバウンディングボックス
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_BOX_COLLIDER_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_BOX_COLLIDER_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Physical\Collider.h>


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  BoxCollider :public  Component, virtual  public  Collider
				{
						friend  class  Collider;

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						BoxCollider();
						BoxCollider( Vector3  center, Vector3  radius );
						virtual  ~BoxCollider();

				private:
						//----------------------------------------------------------------------------------
						// protected variables
						//----------------------------------------------------------------------------------

						static  ComponentId  STATIC_COMPONENT_ID;

						Vector3  m_min;
						Vector3  m_max;

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

				};// class BoxCollider

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_BOX_COLLIDER_