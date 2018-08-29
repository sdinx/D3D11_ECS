//----------------------------------------------------------------------------------
// file : Component.h
// desc : 
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: HandleMessage(const Message&, void*): �e���v���[�g�ł̉ψ����ɕύX����K�v������ ( �������������x��������? )

#ifndef  _INCLUDED_D3D11_UTILITY_COMPONENT_
#define  _INCLUDED_D3D11_UTILITY_COMPONENT_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\IEntity.h>
#include  <D3D11Utility\IComponent.h>
#include  <GameUtility.h>
#include  <list>


namespace  D3D11Utility
{

		class  Component :public  IComponent
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Component()
				{}
				virtual  ~Component()
				{}

		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		public:
				//----------------------------------------------------------------------------------
				// public  variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private  methods
				//----------------------------------------------------------------------------------
				/* NOTHING */

		public:
				//----------------------------------------------------------------------------------
				// public  methods
				//----------------------------------------------------------------------------------
				virtual  void  HandleMessage( const  GameUtility::Message&  msg ) = 0;
				virtual  void  HandleMessage( const  GameUtility::Message&  msg, Value  var ) = 0;// !! need change.
				//virtual  void  FrameUpdate() = 0;// �t���[�����X�V
				virtual  void  Update() = 0;
				//virtual  void  LateUpdate() = 0;
				virtual  void  Rendering()const {};

		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				// 32bit���ł�16byte �A���C�����g�̌x���̂��߂̃I�[�o�[���C�h
				void*  operator  new( std::size_t  block )
				{
						return  _mm_malloc( block, 16 );
				}
				void  operator  delete( void*  ptr )
				{
						_mm_free( ptr );
				}

		};// class Component

}// namespace D3D11Utiltiy

#endif // ! _INCLUDED_D3D11_UTILITY_COMPONENT_