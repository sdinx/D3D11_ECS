//----------------------------------------------------------------------------------
// file: GameObject.h
// desc: this object uses by fps game.
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Renderable.h>


#ifndef  _INCLUDED_D3D11_UTILITY_GAME_OBJECT_
#define  _INCLUDED_D3D11_UTILITY_GAME_OBJECT_


namespace  D3D11Utility
{

		class  GameObject : public  Entity
		{
		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				GameObject( std::string  name, UINT  id, Systems::ComponentManager*  pComponentManagerInstance );
				virtual  ~GameObject();

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		protected:
				//----------------------------------------------------------------------------------
				// protected variables
				//----------------------------------------------------------------------------------

				Transform*  m_transform;
				Renderable*  m_render;

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

				Transform*  GetTransform()
				{
						return  m_transform;
				}
				Renderable*  GetRenderable()
				{
						return  m_render;
				}

		};// class FPSGun

}// namespace D3D11Utility


#endif // ! _INCLUDED_D3D11_UTILITY_GAME_OBJECT_