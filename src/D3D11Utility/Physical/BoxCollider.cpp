//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Physical\BoxCollider.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Physical;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentId  BoxCollider::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


BoxCollider::BoxCollider()
{

}


BoxCollider::BoxCollider( Vector3  center, Vector3  radius ) :
		Collider( center, radius )
{
		m_min = center - radius;
		m_max = center + radius;
}


BoxCollider::~BoxCollider()
{

}


void  BoxCollider::Update()
{

}