//----------------------------------------------------------------------------------
// file : ColliderBox.h
// desc : 3Dでの衝突判定ボックス
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_BOX_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_BOX_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  ColliderBox :public  Component
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						ColliderBox()
						{}
						virtual  ~ColliderBox()
						{}

				private:
						//----------------------------------------------------------------------------------
						// protected variables
						//----------------------------------------------------------------------------------

						static  ComponentId  STATIC_COMPONENT_ID;

						Vector3  m_center;
						Vector3  m_size;

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
						virtual  void  HandleMessage( const  GameUtility::Message&  msg )
						{}
						virtual  void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
						{}
						virtual  void  Update();

				};// class ColliderBox

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_BOX_