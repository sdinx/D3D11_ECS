//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\System\EntityManager.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;

//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
EntityList  EntityManager::m_entityList;


const  EntityId*  EntityManager::CreateEntity( std::string  name )
{
		UINT  maxList = m_entityList.size();
		m_entityList.push_back( new  Entity( name, maxList ) );

		return  ( new  EntityId( maxList ) );
}// end CreateEntity(const Entity*) : const EntityId&


Entity*  EntityManager::GetEntity( const  EntityId&  entityId )
{
		return  m_entityList[entityId.entityId];
}// end GetEntity(const EntityId&) : Entity*


void  ReleaseEntity( Entity*  entity )
{

}// end ReleaseEntity(Entity*)