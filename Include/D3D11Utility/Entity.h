//----------------------------------------------------------------------------------
// file : Entity.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_ENTITY_
#define  _INCLUDED_D3D11_UTILITY_ENTITY_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\System\ComponentManager.h>
#include  <string>


namespace  D3D11Utility
{

		class  Entity :public  IEntity
		{
				// TODO: AddComponent()��������
		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Entity() = delete;
				Entity( std::string  name, UINT  id, ComponentManager*  pComponentManagerInstance );
				virtual  ~Entity();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				BOOL  m_isActive;
				std::string  m_tag;
				ComponentIdList  m_registerList;

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
				inline  BOOL  CompareTag( std::string  tag )const 
				{
						return  ( m_tag == tag ) ? true : false; 
				}
				inline  BOOL  GetActive()const
				{
						return  m_isActive;
				}
				inline  ComponentIdList  GetRegisterComponentList()
				{
						return  m_registerList;
				}
				inline  std::string  GetTag()const 
				{
						return  m_tag; 
				}
				inline  void  SetActive( BOOL  isActive ) 
				{ 
						m_isActive = isActive;
				}
				inline  void  SetTag( std::string  tag )
				{
						m_tag = tag; 
				}
				void  Release();


		public:
				//----------------------------------------------------------------------------------
				// operator
				//----------------------------------------------------------------------------------
				inline  BOOL  operator==( BOOL  isActive ) const 
				{
						return  ( m_isActive == isActive ) ? true : false;
				}
				inline  BOOL  operator!=( BOOL  isActive ) const 
				{
						return  ( m_isActive != isActive ) ? true : false;
				}

				//inline  BOOL  operator==( Entity  entity ) const
				//{
				//		return  ( GetEntityID() == entity.GetEntityID() ) ? true : false;
				//}
				//inline  BOOL  operator!=( Entity  entity ) const
				//{
				//		return  ( GetEntityID() != entity.GetEntityID() ) ? true : false;
				//}

		};// class Entity

}// namespace D3D11Utiltiy

#endif // ! _INCLUDED_D3D11_UTILITY_ENTITY_