//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <d3d11utility\physical\Collider.h>
#include  <d3d11utility\physical\BoxCollider.h>
#include  <d3d11utility\physical\SphereCollider.h>


//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Physical;


bool  Collider::IsIntersect( const  BoxCollider  a, const  BoxCollider  b )
{
		if ( a.m_min.x > b.m_max.x )
				return  false;
		if ( a.m_min.y > b.m_max.y )
				return  false;
		if ( a.m_min.z > b.m_max.z )
				return  false;

		if ( a.m_max.x < b.m_min.x )
				return  false;
		if ( a.m_max.y < b.m_min.y )
				return  false;
		if ( a.m_max.z < b.m_min.z )
				return  false;

		return  true;
}


bool Collider::IsIntersect( const  SphereCollider  a, const  SphereCollider  b )
{

		return  true;
}


bool  Collider::IsIntersect( const  BoxCollider  a, const  SphereCollider  b )
{

		return  true;
}


bool  Collider::IsIntersect( const  SphereCollider  a, const  BoxCollider  b )
{

		return  true;
}