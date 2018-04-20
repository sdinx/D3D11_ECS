//----------------------------------------------------------------------------------
// file : GameObject.h
// desc : final class
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_GAME_UTILITY_GAME_OBJECT_
#define  _INCLUDED_GAME_UTILITY_GAME_OBJECT_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <Mesh.h>
#include  <Scene.h>
#include  <list>


namespace  GameUtility
{

		class  GameObject  final:  public  D3D11Utility::Mesh
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				GameObject() = delete;
				GameObject( std::string  name, std::string  tag = NULL );
				GameObject( D3D11Utility::Scene*  pParentsScene, std::string  name, std::string  tag = NULL );
				~GameObject();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				static  std::list<std::string>  allTag;

				D3D11Utility::Scene*  m_pParentsScene = nullptr;
				std::string  tag;


		public:
				//----------------------------------------------------------------------------------
				// public variables
				//----------------------------------------------------------------------------------


		private:
				//----------------------------------------------------------------------------------
				// private methods
				//----------------------------------------------------------------------------------


		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------
				VOID  SetTag( std::string  objectTag ) { tag = objectTag; }
				VOID  Release();


		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				inline  BOOL  operator==( const  GameObject*  obj ) {
						return  ( this->tag == obj->tag ) ? true : false;
				}
				inline  BOOL  operator==( std::string  tag ) {
						return  ( this->tag == tag ) ? true : false;
				}
				inline  BOOL  operator==( const  D3D11Utility::Scene*  scene ) const{
						return  ( *m_pParentsScene == *scene ) ? true : false;
				}
				inline  BOOL  operator!=( const  GameObject*  obj ) const {
						return  ( this->tag != obj->tag ) ? true : false;
				}
				inline  BOOL  operator!=( std::string  tag ) const {
						return  ( this->tag != tag ) ? true : false;
				}
				inline  BOOL  operator!=( const  D3D11Utility::Scene*  scene ) const {
						return  ( *m_pParentsScene != *scene ) ? true : false;
				}


		};// class  GaneObject

}// namespace  GameUtility

#endif // ! _INCLUDED_GAME_UTILITY_GAME_OBJECT_