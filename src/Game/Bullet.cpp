//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include  <Game/Bullet.h>


//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  GameUtility;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentId  Bullet::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


//Bullet::Bullet()
//{
//
//}


Bullet::Bullet( const  Transform  spawnTransform, const  float  fBulletSpeed ) :
		m_transform( spawnTransform ),
		m_fBulletSpeed( fBulletSpeed )
{

}


Bullet::~Bullet()
{

}


void  Bullet::HandleMessage( const  GameUtility::Message&  msg )
{

}


void  Bullet::Update()
{
		m_transform.SetTranslation( Vector3( 0, 0, m_fBulletSpeed ) );
}