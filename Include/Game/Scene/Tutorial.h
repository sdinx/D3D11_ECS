#ifndef  _INCLUDED_SCENE_TUTORIAL_
#define  _INCLUDED_SCENE_TUTORIAL_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <Game\Scene\BaseScene.h>

namespace  Scene
{

		class  Tutorial:  public  BaseScene
		{
		public:
				Tutorial();
				~Tutorial();

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

		};// class Tutorial

}// namespace Scene

#endif // ! _INCLUDED_SCENE_TUTORIAL_