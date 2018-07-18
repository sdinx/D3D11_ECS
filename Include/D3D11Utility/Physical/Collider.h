//----------------------------------------------------------------------------------
// file : Collider.h
// desc : 衝突判定フラグや中心点を持った基底クラス
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
/* NOTHING */

#ifndef  _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_
#define  _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>


namespace  D3D11Utility
{
		namespace  Physical
		{

				class  Collider :public  Component
				{

				public:
						//----------------------------------------------------------------------------------
						// other
						//----------------------------------------------------------------------------------

						Collider()
						{}
						virtual  ~Collider()
						{}

				private:
						//----------------------------------------------------------------------------------
						// protected variables
						//----------------------------------------------------------------------------------

						static  ComponentId  STATIC_COMPONENT_ID;

						bool  isHit;// フレーム毎に初期化
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
						void  HandleMessage( const  GameUtility::Message&  msg )
						{}
						void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
						{}
						void  Update();

				};// class Collider

		}// namespace Physical
}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_PHYSICAL_COLLIDER_