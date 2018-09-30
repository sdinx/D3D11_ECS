//----------------------------------------------------------------------------------
// file : InputMove.h
// desc : ���͂���g�����X�t�H�[���։e����^����
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: �e�Ǝq�̊֌W�̂Ȃ�悤�ύX.
// TODO: ���[�J���s��ƃ��[���h�s��̌v�Z�p�x���I�u�W�F�N�g���Ƃɒi�K��������.

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
				// TODO: �I�u�W�F�N�g�ɕK�v�ȍ��W��s����̕ϐ���p��

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