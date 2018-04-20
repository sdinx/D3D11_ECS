//----------------------------------------------------------------------------------
// file : Scene.h
// desc : This class is collect the game objects
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_SCENE_
#define  _INCLUDED_D3D11_UTILITY_SCENE_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <Camera.h>
#include  <vector>

//----------------------------------------------------------------------------------
// �O���`
//----------------------------------------------------------------------------------
namespace  GameUtility { class  GameObject; }


namespace  D3D11Utility
{
		// TODO: �`��֐����
		// TODO: IDirect3DRenderer�Ƃ̕`����@���l����

		class  Scene
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Scene();
				~Scene();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				BOOL  m_isEnable;
				UINT  m_nSceneID;

				std::vector<Camera>  m_cameras;
				std::vector<GameUtility::GameObject*>  m_gameObjects;


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
				VOID  CreateCamera( const  Camera&  camera );
				VOID  CreateGameObject();
				VOID  CreateGameObject( UINT  position );
				VOID  CreateGameObject( GameUtility::GameObject  gameObject );
				GameUtility::GameObject*  FindNameGameObject( std::string  name );
				std::vector<GameUtility::GameObject*>  FindTagGameObjects( std::string  tag );
				UINT  GetRootGameObjects( std::vector<GameUtility::GameObject*>  pGameObjects );
				VOID  Rendering();
				VOID  Release();


		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				inline  BOOL  operator==( Scene  scene ) const {
						return  ( this->m_nSceneID == scene.m_nSceneID ) ? true : false;
				}
				inline  BOOL  operator!=( Scene  scene ) const {
						return  ( this->m_nSceneID != scene.m_nSceneID ) ? true : false;
				}


		};// class  Scene

}// namespace  D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_SCENE_