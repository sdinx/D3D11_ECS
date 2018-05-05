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

		};// struct EntityId

		class  IEntity  abstract
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				IEntity() = delete;
				IEntity( const  unsigned  id, ComponentManager*  pComponentManagerInstance ) :
						m_entityId( id ),
						m_pComponentManager( pComponentManagerInstance )
				{}
				virtual  ~IEntity()
				{}


		private:
				//----------------------------------------------------------------------------------
				// private variables
				//----------------------------------------------------------------------------------
				ComponentManager*  m_pComponentManager;

		protected:
				//----------------------------------------------------------------------------------
				// protected variables
				//----------------------------------------------------------------------------------
				const  EntityId  m_entityId;

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
				void  AddComponent( P&&...  param )
				{
						m_pComponentManager->AddComponent<T>( m_entityId, std::forward<P>( param )... );
				}
				template<class  T>
				T*  GetComponent()const
				{
						return  m_pComponentManager->GetComponent<T>( m_entityId );
				}
				inline  const  EntityId  GetEntityId()const
				{
						return  m_entityId;
				}

		};// class IEntity

}// namespace D3D11Utility

#endif // ! _INCLUDED_D3D11_UTILITY_INTERFACE_ENTITY_