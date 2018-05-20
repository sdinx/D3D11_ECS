//----------------------------------------------------------------------------------
// file : IComponent.h
// desc : 
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: �e�̃G���e�B�e�BID��ێ�������

#ifndef  _INCLUDED_D3D11_UTILITY_INTERFACE_COMPONENT_
#define  _INCLUDED_D3D11_UTILITY_INTERFACE_COMPONENT_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\IEntity.h>


namespace  D3D11Utility
{

		/* type defined */
		using  ComponentId = int;

		/* const variables */
		static  const  int  COMPONENT_ID_INVALID = STATIC_ID_INVALID;

		class  IComponent
		{
				friend  class  Systems::ComponentManager;

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				IComponent() :
						m_isActive( true ),
						m_isUpdating( true )
				{}
				virtual  ~IComponent()
				{}

		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------

				ComponentId  m_componentId = -1;

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
				volatile  BOOL&  GetActive()
				{
						return  m_isActive;
				}
				volatile  void  SetUpdating( BOOL  isUpdating )
				{
						m_isActive = isUpdating;
				}

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

		};// class IComponent

}// namespace D3D11Utiltiy

#endif // ! _INCLUDED_D3D11_UTILITY_INTERFACE_COMPONENT_