//----------------------------------------------------------------------------------
// file : Scene.h
// desc : This class is collect the game objects
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_SCENE_
#define  _INCLUDED_D3D11_UTILITY_SCENE_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Camera.h>
#include  <vector>

//----------------------------------------------------------------------------------
// 前提定義
//----------------------------------------------------------------------------------
//namespace  GameUtility { class  GameObject; }


namespace  D3D11Utility
{
		// TODO: 描画関数作る
		// TODO: IDirect3DRendererとの描画方法を考える

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
				//std::vector<GameUtility::GameObject*>  m_gameObjects;


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
				void  CreateCamera( const  Camera&  camera );
				void  CreateGameObject();
				void  CreateGameObject( UINT  position );
				//void  CreateGameObject( GameUtility::GameObject  gameObject );
				//GameUtility::GameObject*  FindNameGameObject( std::string  name );
				//std::vector<GameUtility::GameObject*>  FindTagGameObjects( std::string  tag );
				//UINT  GetRootGameObjects( std::vector<GameUtility::GameObject*>  pGameObjects );
				void  Rendering();
				void  Release();


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