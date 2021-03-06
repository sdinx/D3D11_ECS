//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\SystemManager.h>
#include  <D3D11Utility\Systems\ComponentManager.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Systems;


SystemManager::SystemManager( ComponentManager*  pComponentManagerInstance, EntityManager*  pEntityManager ) :
		m_pComponentManager( pComponentManagerInstance ),
		m_pEntityManager( pEntityManager )
{

}


SystemManager::~SystemManager()
{
		Release();
}


void  SystemManager::Update( float  ms )
{
		for ( auto system : m_systemList )
				system->Update( ms );
}


void  SystemManager::Release()
{
		for ( auto system : m_systemList )
		{
				SafeDelete( system );
				//system->Release();
		}
		m_systemList.clear();
}