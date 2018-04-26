//----------------------------------------------------------------------------------
// file : EntityManager.h
// desc : Entity class and EntityID management class
//----------------------------------------------------------------------------------
#ifndef  _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <list>
#include  <vector>


namespace  D3D11Utility
{
		class  Entity;
		struct  EntityId;

		// TODO: Entity の生成とメッセージ処理とか考える
		class  EntityManager
		{

		public:
				//----------------------------------------------------------------------------------
				// other
				//----------------------------------------------------------------------------------
				EntityManager();
				~EntityManager();


		private:
				//----------------------------------------------------------------------------------
				// private  variables
				//----------------------------------------------------------------------------------
				using  EntityList = std::vector<Entity*>;
				EntityList  m_entityList;


		public:
				//----------------------------------------------------------------------------------
				// public  variables
				//----------------------------------------------------------------------------------
				/* NOTHING */

		private:
				//----------------------------------------------------------------------------------
				// private  functions
				//----------------------------------------------------------------------------------
				/* NOTHING */

		public:
				//----------------------------------------------------------------------------------
				// public  functions
				//----------------------------------------------------------------------------------
				const  EntityId&  CreateEntity( std::string  name );
				const  Entity*  GetEntity( const  EntityId&  entityId );
				const  EntityId  GetEntityId( const  Entity&  entity );
				void  ReleaseEntity( Entity*  entity );

		};

}


#endif // ! _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_