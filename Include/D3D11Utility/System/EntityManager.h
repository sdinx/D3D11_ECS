//----------------------------------------------------------------------------------
// file : EntityManager.h
// desc : Entity class and EntityID management class
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Interface.h>
#include  <list>
#include  <vector>


namespace  D3D11Utility
{
		// TODO: Entity の生成とメッセージ処理とか考える

		//----------------------------------------------------------------------------------
		// 前提定義
		//----------------------------------------------------------------------------------
		class  Entity;
		struct  EntityId;

		//----------------------------------------------------------------------------------
		// type defined
		//----------------------------------------------------------------------------------
		using  EntityList = std::vector<Entity*>;


		class  EntityManager
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				EntityManager() = delete;
				EntityManager( ComponentManager*  pComponentManagerInstance ) :
						m_pComponentManager( pComponentManagerInstance )
				{}
				~EntityManager()
				{}


		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------
				EntityList  m_entityList;
				ComponentManager*  m_pComponentManager = nullptr;

		public:
				//----------------------------------------------------------------------------------
				// public  variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private  methods
				//----------------------------------------------------------------------------------

		public:
				//----------------------------------------------------------------------------------
				// public  methods
				//----------------------------------------------------------------------------------
				const  EntityId  CreateEntity( std::string  name );
				const  EntityId  GetEntityId( const  Entity&  entity );
				Entity*  GetEntity( const  EntityId  entityId );
				void  ReleaseEntity( Entity*  entity );

		};

}


#endif // ! _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_