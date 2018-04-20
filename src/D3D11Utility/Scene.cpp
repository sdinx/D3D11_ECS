//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <Scene.h>
#include  <GameObject.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;


Scene::Scene()
{

}


Scene::~Scene()
{
		Release();
}


VOID  Scene::CreateCamera( const  Camera&  camera )
{
		m_cameras.push_back( camera );
}


VOID  Scene::CreateGameObject()
{
		m_gameObjects.push_back( new  GameUtility::GameObject( this, "name", "tag" ) );
}


VOID  Scene::CreateGameObject( UINT  position )
{

		m_gameObjects.insert( m_gameObjects.begin() + position, new  GameUtility::GameObject( this, "name", "tag" ) );
}


VOID  Scene::CreateGameObject( GameUtility::GameObject  gameObject )
{
		m_gameObjects.push_back( new  GameUtility::GameObject( gameObject ) );

}


GameUtility::GameObject*  Scene::FindNameGameObject( std::string  name )
{
		DWORD  maxGameObjects = m_gameObjects.size();
		GameUtility::GameObject*  retGameObject = NULL;

		for ( DWORD i = 0; i < maxGameObjects; i++ )
		{
				if ( m_gameObjects[i]->ToString() == name )
				{
						retGameObject = m_gameObjects[i];
						break;
				}// end if
		}// end for

		return  retGameObject;
}


std::vector<GameUtility::GameObject*>  Scene::FindTagGameObjects( std::string  tag )
{
		DWORD  maxGameObjects = m_gameObjects.size();
		std::vector<GameUtility::GameObject*>  retGameObjects;

		for ( DWORD i = 0; i < maxGameObjects; i++ )
		{
				if ( *m_gameObjects[i] == tag )
				{
						retGameObjects.push_back( m_gameObjects[i] );
				}// end if
		}// end for

		return  retGameObjects;
}


UINT  Scene::GetRootGameObjects( std::vector<GameUtility::GameObject*>  pGameObjects )
{
		pGameObjects = m_gameObjects;
		return  m_gameObjects.size();
}


VOID  Scene::Rendering()
{
		DWORD  maxRenderMeshes = m_gameObjects.size();

		for ( DWORD i = 0; i < maxRenderMeshes; i++ )
		{
				m_gameObjects[i]->Rendering();
		}
}


VOID  Scene::Release()
{

}