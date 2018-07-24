//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Physical\BoxCollider2D.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Physical;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentId  BoxCollider2D::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


BoxCollider2D::BoxCollider2D()
{

}


BoxCollider2D::BoxCollider2D( Vector2  center, Vector2  radius ) :
		Collider2D( center, radius )
{

}


BoxCollider2D::~BoxCollider2D()
{

}


void  BoxCollider2D::Update()
{

}