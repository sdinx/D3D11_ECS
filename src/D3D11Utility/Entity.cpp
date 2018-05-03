//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;


Entity::Entity( std::string  name, const  UINT  id ) :
		IEntity( id ),
		m_isActive( true ),
		m_tag( "none" )
{

}// end constractor(std::string, const  UINT)


Entity::~Entity()
{

}// end destractor


void  Entity::Release()
{

}// end Release()