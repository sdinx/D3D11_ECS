#ifndef  _INCLUDED_SCENE_TUTORIAL_
#define  _INCLUDED_SCENE_TUTORIAL_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <Game\Scene\BaseScene.h>

namespace  Scene
{

		class  Tutorial:  public  _BaseScene
		{
		public:
				Tutorial();
				~Tutorial();

		public:
				//----------------------------------------------------------------------------------
				// public methods
				//----------------------------------------------------------------------------------

				void  Awake();
				void  Init();
				void  Update();

		};// class Tutorial

}// namespace Scene

#endif // ! _INCLUDED_SCENE_TUTORIAL_