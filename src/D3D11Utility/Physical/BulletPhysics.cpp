//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Physical\BulletPhysics.h>
#include  <D3D11Utility\Systems\BulletEngine.h>
#include  <D3D11Utility\Systems\ComponentManager.h>


//----------------------------------------------------------------------------------
// pragma
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  D3D11Utility::Physical;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentId  BulletPhysics::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


BulletPhysics::BulletPhysics( btRigidBody*  pRigidBody, const  Systems::BulletEngine*  pBulletEngine ) :
		m_pRigidBody( pRigidBody ),
		m_pBulletEngine( pBulletEngine )
{

}


BulletPhysics::~BulletPhysics()
{
		Release();
}


void  BulletPhysics::HandleMessage( const  GameUtility::Message&  msg )
{

}


void  BulletPhysics::Update()
{
		Transform  trans = GetComponent<Transform>();
		trans.Update();
		m_pRigidBody->setWorldTransform( BulletConvertTransform( trans ) );
}


void  BulletPhysics::Release()
{

}