//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Systems\ComponentManager.h>
#include  <Game/GameObject.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  GameUtility;
using  namespace  DirectX;


GameObject::GameObject( std::string  name, UINT  id, Systems::ComponentManager*  pComponentManagerInstance ) :
		Entity( name, id, pComponentManagerInstance )
{

}


GameObject::~GameObject()
{

}