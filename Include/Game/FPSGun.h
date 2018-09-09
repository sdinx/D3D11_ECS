//----------------------------------------------------------------------------------
// file: Bullet.h
// desc: èeÇ™éÀèoÇ∑ÇÈíeÉNÉâÉX
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>


#ifndef  _INCLUDED_D3D11_UTILITY_GAME_FPSGUN_
#define  _INCLUDED_D3D11_UTILITY_GAME_FPSGUN_


namespace  D3D11Utility
{

		class  FPSGun :public  Component
		{
		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				FPSGun();
				virtual  ~FPSGun();

		private:
				//----------------------------------------------------------------------------------
				// private variables
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

				/* static */
				static  ComponentId  GetStaticComponentId()
				{
						return  STATIC_COMPONENT_ID;
				}
				static  void  SetStaticComponentId( ComponentId  id )
				{
						if ( STATIC_COMPONENT_ID == STATIC_ID_INVALID )
						{
								STATIC_COMPONENT_ID = id;
								// TODO: need output debug string.
						}
				}

				/* derived virtual */
				void  HandleMessage( const  GameUtility::Message&  msg );
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
				{}
				void  Update();

		};// class FPSGun

}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GAME_FPSGUN_