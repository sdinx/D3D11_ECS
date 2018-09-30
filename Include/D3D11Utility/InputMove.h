//----------------------------------------------------------------------------------
// file : InputMove.h
// desc : 入力からトランスフォームへ影響を与える
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: 親と子の関係のなるよう変更.
// TODO: ローカル行列とワールド行列の計算頻度をオブジェクトごとに段階分けする.

#ifndef  _INCLUDED_D3D11_UTILITY_INPUT_MOVE_
#define  _INCLUDED_D3D11_UTILITY_INPUT_MOVE_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility/Camera.h>


namespace  D3D11Utility
{

		class  InputMove :public  Component
		{
				// TODO: オブジェクトに必要な座標や行列情報の変数を用意

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				InputMove( Transform*  target, Camera*  camera );

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				static  ComponentId  STATIC_COMPONENT_ID;
				Transform*  m_target;
				Camera*  m_camera;

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
				void  HandleMessage( const  GameUtility::Message&  msg ) {}
				void  HandleMessage( const  GameUtility::Message&  msg, Value  var )
				{}
				void  Update();

		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				/* NOTHING */

		};

}

#endif // ! _INCLUDED_D3D11_UTILITY_INPUT_MOVE_