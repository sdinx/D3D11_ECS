#ifndef  _INCLUDED_SCENE_TUTORIAL_
#define  _INCLUDED_SCENE_TUTORIAL_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <game\Scene\BaseScene.h>
#include  <d3d11utility\components/Camera.h>
#include  <d3d11utility\Entity.h>
#include  <d3d11utility\Interface.h>

namespace  Scene
{

		class  Tutorial:  public  BaseScene
		{
		public:
				Tutorial();
				~Tutorial();

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				std::unique_ptr<D3D11Utility::Systems::ComponentManager>  m_pComponentManager;
				std::unique_ptr<D3D11Utility::Systems::EntityManager>  m_pEntityManager;
				std::unique_ptr<D3D11Utility::Systems::SystemManager>  m_pSystemManager;
				std::shared_ptr<D3D11Utility::Systems::IDirect3DRenderer>  m_pd3dRenderer;
				std::shared_ptr<D3D11Utility::Systems::TextureManager>  m_pTextureManager;
				D3D11Utility::Camera*  m_FPSCamera = nullptr;
				D3D11Utility::Systems::DebugSystem*  m_pDebugSystem;

				// ゲームオブジェクト
				D3D11Utility::Entity*  m_playerEntity;

		public:
				//----------------------------------------------------------------------------------
				// public variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private methods
				//----------------------------------------------------------------------------------

				void  InputFPSCamera();

		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------
				
				static  Tutorial&  GetInstance()
				{
						static  Tutorial  instance;
						return  instance;
				}
				void  Awake();
				void  Init();
				void  Update();
				void  Release();

		};// class Tutorial

}// namespace Scene

#endif // ! _INCLUDED_SCENE_TUTORIAL_