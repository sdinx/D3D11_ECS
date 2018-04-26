//----------------------------------------------------------------------------------
// file : Entity.h
// desc : 
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_ENTITY_
#define  _INCLUDED_D3D11_UTILITY_ENTITY_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Component.h>
#include  <D3D11Utility\D3D11Utility.h>
#include  <D3D11Utility\System\EntityManager.h>
#include  <string>


namespace  D3D11Utility
{

		struct  EntityId
		{
				const  unsigned  entityId;

				EntityId( const  unsigned  id ) :
						entityId( id )
				{}

				inline  bool  operator==( EntityId  id )const
				{
						return  ( entityId == id.entityId ) ? true : false;
				}
				inline  bool  operator!=( EntityId  id )const
				{
						return  ( entityId != id.entityId ) ? true : false;
				}
				inline  bool  operator<( EntityId  id )const
				{
						return  ( entityId < id.entityId ) ? true : false;
				}
				inline  bool  operator>( EntityId  id )const
				{
						return  ( entityId > id.entityId ) ? true : false;
				}
				inline  bool  operator<=( EntityId  id )const
				{
						return  ( entityId <= id.entityId ) ? true : false;
				}
				inline  bool  operator>=( EntityId  id )const
				{
						return  ( entityId >= id.entityId ) ? true : false;
				}
		};

		class  Entity
		{
				// TODO: GetComponent()ÇçlÇ¶ÇÈ
				// TODO: AddComponent()èëÇ±Ç§Ç∫
		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				Entity();
				Entity( std::string  name );
				~Entity();


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
				inline  const  EntityId  GetEntityId()const
				{ 
						return  0;
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

				void  AddComponent( Component*  component );
				template<typename  T>  T  GetComponent()const;
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