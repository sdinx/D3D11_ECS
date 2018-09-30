//----------------------------------------------------------------------------------
// File : StateTransition.h
// Desc : 状態遷移などで使う基底クラス
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_STATE_TRANSITION_
#define  _INCLUDED_STATE_TRANSITION_

template  <class  STATE>
class  _StateTransition
{
public:
		typedef void( STATE::*FNSTATE )( );  // ゲームシーンのメソッド型

		_StateTransition( const _StateTransition& ) = delete;// コピーコンストラクタ禁止
		static _StateTransition& GetInstance() 
		{
				static _StateTransition instance;
				return instance;
		}

protected:
		_StateTransition() {}	// 外部でのインスタンス作成禁止
		virtual ~_StateTransition() {}

private:
		//----------------------------------------------------------------------------------
		// private variables
		//----------------------------------------------------------------------------------

		STATE*  m_pHandle;
		FNSTATE  m_fnNowState;  // ゲームシーンのメソッドポインタ

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
				// ゲームシーン更新
				( m_pHandle->*m_fnNowState )( );  // ゲームシーンの呼び出し
		}
		inline  virtual  void  SetSceneState( STATE  &object ) 
		{  
				// ゲームシーン変更
				m_pHandle = &object;
		}
};// class _StateTransition

#endif // !_INCLUDED_STATE_TRANSITION_