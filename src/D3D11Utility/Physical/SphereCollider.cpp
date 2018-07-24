//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Physical\SphereCollider.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  Physical;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentId  SphereCollider::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


SphereCollider::SphereCollider()
{

}


SphereCollider::SphereCollider( Vector3  center, Vector3  radius ) :
		Collider( center, radius )
{

}


SphereCollider::~SphereCollider()
{

}


void  SphereCollider::Update()
{

}