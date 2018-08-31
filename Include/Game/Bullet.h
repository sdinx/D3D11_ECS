//----------------------------------------------------------------------------------
// file: Bullet.h
// desc: èeÇ™éÀèoÇ∑ÇÈíeÉNÉâÉX
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <Game\GameObject.h>


#ifndef  _INCLUDED_D3D11_UTILITY_GAME_BULLET_
#define  _INCLUDED_D3D11_UTILITY_GAME_BULLET_


namespace  D3D11Utility
{

		class  Bullet :public  GameObject
		{
		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				Bullet() = delete;
				Bullet( const  Transform  spawnTransform, const  float  fBulletSpeed );
				virtual  ~Bullet();

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;

				FLOAT  m_fBulletSpeed;
				UINT  m_nOwner;

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

		};// class Bullet

}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GAME_BULLET_