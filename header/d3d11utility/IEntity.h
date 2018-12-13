//----------------------------------------------------------------------------------
// file : IEntity.h
// desc : 
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// comments
//----------------------------------------------------------------------------------
// TODO: テンプレート使用時の警告の解消

#ifndef  _INCLUDED_D3D11_UTILITY_INTERFACE_ENTITY_
#define  _INCLUDED_D3D11_UTILITY_INTERFACE_ENTITY_

//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Interface.h>
#include  <string>


namespace  D3D11Utility
{

		struct  EntityId
		{

				unsigned  index;
				const  IEntity*  parent;

				EntityId()
				{}
				EntityId( const  unsigned  id, const  IEntity*  entity ) :
						index( id ),
						parent( entity )
				{}

				inline  bool  operator==( EntityId  id )const
				{
						return  ( index == id.index ) ? true : false;
				}
				inline  bool  operator!=( EntityId  id )const
				{
						return  ( index != id.index ) ? true : false;
				}
				inline  bool  operator<( EntityId  id )const
				{
						return  ( index < id.index ) ? true : false;
				}
				inline  bool  operator>( EntityId  id )const
				{
						return  ( index > id.index ) ? true : false;
				}
				inline  bool  operator<=( EntityId  id )const
				{
						return  ( index <= id.index ) ? true : false;
				}
				inline  bool  operator>=( EntityId  id )const
				{
						return  ( index >= id.index ) ? true : false;
				}

		};// struct EntityId

		class  IEntity  abstract
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------

				IEntity() = delete;
				IEntity( std::string  name, unsigned  id, Systems::ComponentManager*  pComponentManagerInstance ) :
						m_name( name ),
						m_nameHash( std::hash<std::string>()( name ) ),
						m_entityId( id, this ),
						m_pComponentManager( pComponentManagerInstance )
				{}
				virtual  ~IEntity()
				{}

		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------

				Systems::ComponentManager*  m_pComponentManager;

		protected:
				//----------------------------------------------------------------------------------
				// protected variables
				//----------------------------------------------------------------------------------
				const  EntityId  m_entityId;
				const  std::string  m_name;
				const  size_t  m_nameHash;

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

				template<class  T, typename  ...P>
				T*  AddComponent( P&&...  param )
				{
						return  m_pComponentManager->AddComponent<T>( m_entityId, std::forward<P>( param )... );
				}
				template<class  T>
				T*  GetComponent()const
				{
						return  m_pComponentManager->GetComponent<T>( m_entityId );
				}
				std::string  GetName() const { return  m_name; }
				inline  const  EntityId  GetEntityId()const { return  m_entityId; }

				inline  bool  operator==( const  std::string  name ) const
				{
						return  ( m_nameHash == std::hash<std::string>()( name ) ) ? true : false;
				}
				inline  bool  operator!=( const  std::string  name ) const
				{
						return  ( m_nameHash != std::hash<std::string>()( name ) ) ? true : false;
				}

		};// class IEntity

}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_INTERFACE_ENTITY_