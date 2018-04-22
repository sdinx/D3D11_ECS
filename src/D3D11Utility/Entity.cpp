//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Entity.h>

//----------------------------------------------------------------------------------
// using namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;


Entity::Entity()
{

}// end default constractor


Entity::Entity( std::string  name )
{

}// end constractor(std::string)


Entity::~Entity()
{

}// end destractor


void  Entity::AddComponent( Component*  component )
{

}// end AddComponent(Component*)


template<typename  T>
T  Entity::GetComponent()const
{

}// end GetComponent()const


void  Entity::Release()
{

}// end Release()