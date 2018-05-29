//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Physical\ColliderBox.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Physical;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentId  ColliderBox::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


ColliderBox::ColliderBox()
{

}


ColliderBox::ColliderBox( Vector3  center, Vector3  size ) :
		m_center( center ),
		m_size( size )
{

}


ColliderBox::~ColliderBox()
{

}


void  ColliderBox::Update()
{

}