//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Transform.h>


//----------------------------------------------------------------------------------
// using
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  DirectX;


//----------------------------------------------------------------------------------
// static variables
//----------------------------------------------------------------------------------
ComponentId  Transform::STATIC_COMPONENT_ID = STATIC_ID_INVALID;


Transform::Transform() :
		m_localScale( 1, 1, 1 ),
		m_position( 0, 0, 0 ),
		m_translation( 0, 0, 0 ),
		m_euler( 0, 0, 0 ),
		m_scale( 1, 1, 1 ),
		m_pParent( nullptr )
{
		HandleMessage( GameUtility::Message( MSG_UPDATE_LOCAL ) );
}


Transform::Transform( Transform*  parent ) :
		m_localScale( 1, 1, 1 ),
		m_position( 0, 0, 0 ),
		m_translation( 0, 0, 0 ),
		m_euler( 0, 0, 0 ),
		m_scale( 1, 1, 1 ),
		m_pParent( parent )
{
		HandleMessage( GameUtility::Message( MSG_UPDATE_LOCAL ) );
}


void  Transform::HandleMessage( const  GameUtility::Message&  msg )
{
		switch ( msg.messageType )
		{
		case  MSG_UPDATE_LOCAL:
				{
						XMMATRIX  localWorld;
						Vector3  r( ToRadian( m_localEuler.x ), ToRadian( m_localEuler.y ), ToRadian( m_localEuler.z ) );

						localWorld = XMMatrixMultiply( XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &r ) ), XMMatrixTranslationFromVector( XMLoadFloat3( &m_localPosition ) ) );
						localWorld = XMMatrixMultiply( XMMatrixScalingFromVector( XMLoadFloat3( &m_localScale ) ), localWorld );
						XMStoreFloat4x4( &m_localWorld, localWorld );

				}// case MSG_UPDATE_LOCAL
		}// end switch

}


void  Transform::Update()
{
		XMMATRIX  mtxPos, mtxTrans, mtxRotate, mtxScale;
		Vector3  r( ToRadian( m_euler.x ), ToRadian( m_euler.y ), ToRadian( m_euler.z ) );

		// 行列変換
		mtxPos = XMMatrixTranslationFromVector( XMLoadFloat3( &m_position ) );
		mtxRotate = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &r ) );
		mtxTrans = XMMatrixTranslationFromVector( XMLoadFloat3( &m_translation ) );
		mtxScale = XMMatrixScalingFromVector( XMLoadFloat3( &m_scale ) );

		// 行列計算
		mtxPos = XMMatrixMultiply( mtxRotate, mtxPos );
		mtxPos = XMMatrixMultiply( mtxTrans, mtxPos );
		mtxPos = XMMatrixMultiply( mtxScale, mtxPos );

		// ワールド行列の更新
		XMStoreFloat4x4( &m_world, mtxPos );

		// 現在座標を計算後の座標に更新
		m_position.x = m_world._41;
		m_position.y = m_world._42;
		m_position.z = m_world._43;

		// 移動量の初期化
		m_translation = Vector3( 0, 0, 0 );
}


const  Matrix4x4  Transform::GetWorldMatrix()
{
		DirectX::XMMATRIX  mtxWorld = DirectX::XMLoadFloat4x4( &m_world );
		mtxWorld = DirectX::XMMatrixMultiply( MultiplyRootTransform( this ), mtxWorld );
		mtxWorld = DirectX::XMMatrixMultiply( DirectX::XMLoadFloat4x4( &m_localWorld ), mtxWorld );
		Matrix4x4  matrix4x4;
		DirectX::XMStoreFloat4x4( &matrix4x4, mtxWorld );

		return  matrix4x4;
}


DirectX::XMMATRIX  Transform::MultiplyRootTransform( Transform*  parent )
{
		DirectX::XMMATRIX  mtxWorld = DirectX::XMLoadFloat4x4( &parent->GetWorld() );
		DirectX::XMMATRIX  mtxParent;

		if ( parent->m_pParent != nullptr )
				mtxParent = MultiplyRootTransform( parent->m_pParent );
		else
				return  mtxWorld;

		return  DirectX::XMMatrixMultiply( mtxParent, mtxWorld );
}