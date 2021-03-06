//----------------------------------------------------------------------------------
// file : IComponent.h
// desc : 
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: 親のエンティティIDを保持させる

#ifndef  _INCLUDED_D3D11_UTILITY_INTERFACE_COMPONENT_
#define  _INCLUDED_D3D11_UTILITY_INTERFACE_COMPONENT_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility/IEntity.h>
#include  <game/GameUtility.h>


namespace  D3D11Utility
{
		struct  Message
		{
				Message()
				{}
				Message( unsigned  int  msg )
				{
						messageType = msg;
				}

				unsigned  int  messageType;
		};// struct Message

		/* type defined */
		using  ComponentId = int;

		/* const variables */
		static  const  ComponentId  COMPONENT_ID_INVALID = STATIC_ID_INVALID;

		class  IComponent :public  Object
		{
				friend  class  Systems::ComponentManager;

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				IComponent() :Object(),
						m_isActive( true ),
						m_isUpdating( true )
				{}
				virtual  ~IComponent()
				{}

		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------

				ComponentId  m_componentId = STATIC_ID_INVALID;

		protected:
				//----------------------------------------------------------------------------------
				// protected  variables
				//----------------------------------------------------------------------------------

				BOOL  m_isActive;
				BOOL  m_isUpdating;
				EntityId  m_parentsEntityId;
				Systems::ComponentManager*  m_pComponentManager = nullptr;

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

				template<class  T, typename  ...P>
				T*  AddComponent( P&&... param )
				{
						return  m_pComponentManager->AddComponent<T>( m_parentsEntityId, std::forward<P>( param )... );
				}
				template<typename  T>
				T*  GetComponent()const
				{
						return  m_pComponentManager->GetComponent<T>( m_parentsEntityId );
				}

				volatile  const  EntityId  GetEntityId()
				{
						return  m_parentsEntityId;
				}
				volatile  void  SetActive( BOOL  isActive )
				{
						m_isActive = isActive;
				}
				volatile  BOOL  GetActive()
				{
						return  m_isActive;
				}
				volatile  void  SetUpdating( BOOL  isUpdating )
				{
						m_isActive = isUpdating;
				}

				virtual  void  HandleMessage( const  Message&  msg ) = 0;
				//virtual  void  HandleMessage( const  Message&  msg, Value  var ) = 0;// !! need change.
				//virtual  void  BeforeUpdate() = 0;
				virtual  void  Update() = 0;
				//virtual  void  AfterUpdate() = 0;
				virtual  void  Release() {}

		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------

				// 32bit環境での16byte アライメントの警告のためのオーバーライド
				void*  operator  new( std::size_t  block )
				{
						return  _mm_malloc( block, 16 );
				}
				void  operator  delete( void*  ptr )
				{
						_mm_free( ptr );
				}

		};// class IComponent

}// namespace D3D11Utiltiy

#endif // ! _INCLUDED_D3D11_UTILITY_INTERFACE_COMPONENT_