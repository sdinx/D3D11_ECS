//----------------------------------------------------------------------------------
// file : EntityManager.h
// desc : Entity class and EntityID management class
//----------------------------------------------------------------------------------
#ifndef  _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_
#define  _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <Entity.h>
#include  <EntityId.h>
#include  <list>
#include  <vector>


namespace  D3D11Utility
{

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
				std::list<EntityId>  m_entityIds;
				std::vector<Entity>  m_entitys;


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
				EntityId  CreateEntity( Entity*  entity );


		};

}


#endif // ! _INCLUDED_D3D11_UTILITY_ENTITY_MANAGER_