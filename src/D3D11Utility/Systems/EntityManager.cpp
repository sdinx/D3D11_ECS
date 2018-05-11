//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\EntityManager.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


const  EntityId  EntityManager::CreateEntity( std::string  name )
{
		UINT  maxList = m_entityList.size();
		m_entityList.push_back( new  Entity( name, maxList, m_pComponentManager ) );
		m_pComponentManager->AddEntity( EntityId( maxList ) );

		return  EntityId( maxList );
}// end CreateEntity(const Entity*) : const EntityId&


const  EntityId  EntityManager::FindName( std::string  name )
{
		return  0;
}


Entity*  EntityManager::GetEntity( const  EntityId  entityId )
{
		return  m_entityList[entityId.entityId];
}// end GetEntity(const EntityId&) : Entity*


void  EntityManager::ReleaseEntity( Entity*  entity )
{

}// end ReleaseEntity(Entity*)


void  EntityManager::Release()
{
		m_entityList.clear();
		m_entityList.shrink_to_fit();
}// end Release()