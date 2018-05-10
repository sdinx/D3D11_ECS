//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>
#include  <D3D11Utility\Systems\EntityManager.h>

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;


Entity::Entity( std::string  name, UINT  id, Systems::ComponentManager*  pComponentManagerInstance ) :
		/* Initialize */
		IEntity( id, pComponentManagerInstance ),
		m_isActive( true ),
		m_tag( "none" )
{
		/* NOTHING */
}// end constractor(std::string, const  UINT)


Entity::~Entity()
{

}// end destractor


void  Entity::Release()
{

}// end Release()