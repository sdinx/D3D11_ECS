//----------------------------------------------------------------------------------
// File : StateTransition.h
// Desc : ��ԑJ�ڂȂǂŎg�����N���X
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_STATE_TRANSITION_
#define  _INCLUDED_STATE_TRANSITION_

template  <class  STATE>
class  _StateTransition
{
public:
		typedef void( STATE::*FNSTATE )( );  // �Q�[���V�[���̃��\�b�h�^

		_StateTransition( const _StateTransition& ) = delete;// �R�s�[�R���X�g���N�^�֎~
		static _StateTransition& GetInstance() 
		{
				static _StateTransition instance;
				return instance;
		}

protected:
		_StateTransition() {}	// �O���ł̃C���X�^���X�쐬�֎~
		virtual ~_StateTransition() {}

private:
		//----------------------------------------------------------------------------------
		// private variables
		//----------------------------------------------------------------------------------

		STATE*  m_pHandle;
		FNSTATE  m_fnNowState;  // �Q�[���V�[���̃��\�b�h�|�C���^

public:
		//----------------------------------------------------------------------------------
		// public methods
		//----------------------------------------------------------------------------------

		inline  virtual  void  SetMethodState( FNSTATE  fnMethod ) 
		{
				m_fnNowState = fnMethod;
		}
		inline  virtual  void  Update() 
		{  
				// �Q�[���V�[���X�V
				( m_pHandle->*m_fnNowState )( );  // �Q�[���V�[���̌Ăяo��
		}
		inline  virtual  void  SetSceneState( STATE  &object ) 
		{  
				// �Q�[���V�[���ύX
				m_pHandle = &object;
		}
};// class _StateTransition

#endif // !_INCLUDED_STATE_TRANSITION_