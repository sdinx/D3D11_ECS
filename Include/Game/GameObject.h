//----------------------------------------------------------------------------------
// file: GameObject.h
// desc: this object uses by fps game.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Renderable.h>


#ifndef  _INCLUDED_D3D11_UTILITY_GAME_OBJECT_
#define  _INCLUDED_D3D11_UTILITY_GAME_OBJECT_


namespace  D3D11Utility
{

		class  GameObject : public  Component
		{
		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				GameObject();
				virtual  ~GameObject();

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;

		protected:
				//----------------------------------------------------------------------------------
				// protected variables
				//----------------------------------------------------------------------------------

				Transform  m_transform;
				Renderable  m_render;

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
				virtual  void  HandleMessage( const  GameUtility::Message&  msg );
				virtual  void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
				{}
				virtual  void  Update();
				Transform&  GetTransform()
				{
						return  m_transform;
				}
				Renderable&  GetRenderable()
				{
						return  m_render;
				}

		};// class FPSGun

}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GAME_OBJECT_