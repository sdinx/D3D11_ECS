//----------------------------------------------------------------------------------
// file : Entity.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_ENTITY_
#define  _INCLUDED_D3D11_UTILITY_ENTITY_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility.h>
#include  <d3d11utility\Systems\ComponentManager.h>
#include  <string>


namespace  D3D11Utility
{

		class  Entity :public  IEntity
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				Entity() = delete;
				Entity( std::string  name, UINT  id, Systems::ComponentManager*  pComponentManagerInstance );
				virtual  ~Entity();


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				BOOL  m_isActive;
				std::string  m_tag;

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

				inline  BOOL  operator==( const  Entity&  entity ) const
				{
						return  ( m_entityId == entity.m_entityId ) ? true : false;
				}
				inline  BOOL  operator!=( const  Entity&  entity ) const
				{
						return  ( m_entityId != entity.m_entityId ) ? true : false;
				}

				inline  BOOL  operator==( const  std::string  name ) const
				{
						return  ( m_nameHash == std::hash<std::string>()( name ) ) ? true : false;
				}
				inline  BOOL  operator!=( const  std::string  name ) const
				{
						return  ( m_nameHash != std::hash<std::string>()( name ) ) ? true : false;
				}

		};// class Entity

}// namespace D3D11Utiltiy

#endif // ! _INCLUDED_D3D11_UTILITY_ENTITY_