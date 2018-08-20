//----------------------------------------------------------------------------------
// file: Base.h
// desc: ゲームシーンの基底クラス
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_SCENE_BASE_SCENE_
#define  _INCLUDED_SCENE_BASE_SCENE_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <StateTransition.h>


namespace  Scene
{
		using  GameScene = _StateTransition<Scene::_BaseScene>;

		class  _BaseScene
		{
		protected:
				_BaseScene() :
						m_isAwake( true ),
						m_isInit( true )
				{}

		protected:
				//----------------------------------------------------------------------------------
				// protected variables
				//----------------------------------------------------------------------------------

				bool  m_isInit;
				bool  m_isAwake;

		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------

				virtual  void  Awake() = 0;
				virtual  void  Init() = 0;
				virtual  void  Update() = 0;

		};// class _BaseScene

}// namespace Scene

#endif // ! _INCLUDED_SCENE_BASE_SCENE_