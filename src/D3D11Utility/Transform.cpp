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


void  Transform::Update()
{
		XMMATRIX  mtxPos, mtxAngle, mtxTrans, mtxRotate, mtxScale;

		mtxPos = XMMatrixTranslationFromVector( XMLoadFloat3( &m_position ) );
		mtxAngle = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &m_angle ) );
		mtxTrans = XMMatrixTranslationFromVector( XMLoadFloat3( &m_translation ) );
		mtxRotate = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &m_rotation ) );
		mtxScale = XMMatrixScalingFromVector( XMLoadFloat3( &m_scale ) );

		XMStoreFloat4x4( &m_localWorld, mtxTrans );
		XMStoreFloat4x4( &m_localWorld, mtxRotate );
		XMStoreFloat4x4( &m_localWorld, mtxScale );
		XMStoreFloat4x4( &m_localWorld, mtxPos );
		mtxAngle = XMMatrixMultiply( mtxAngle, XMMatrixTranslation( 0, 0, 0 ) );
		mtxPos = XMMatrixMultiply( mtxTrans, mtxPos );
		mtxPos = XMMatrixMultiply( mtxRotate, mtxPos );
		mtxPos = XMMatrixMultiply( mtxScale, mtxPos );
		mtxPos = XMMatrixMultiply( mtxAngle, mtxPos );

		// ワールド行列の更新
		XMStoreFloat4x4( &m_localWorld, mtxPos );

		// 現在座標を計算後の座標に更新
		m_position.x = m_localWorld._41;
		m_position.y = m_localWorld._42;
		m_position.z = m_localWorld._43;

		// 移動量の初期化
		m_translation = Vector3( 0, 0, 0 );
}