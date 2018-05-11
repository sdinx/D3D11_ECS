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


SystemManager::SystemManager( ComponentManager*  pComponentManagerInstance ) :
		m_pComponentManager( pComponentManagerInstance )
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
		m_systemList.clear();
}