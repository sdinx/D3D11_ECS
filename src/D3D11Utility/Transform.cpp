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

		// 行列変換
		mtxPos = XMMatrixTranslationFromVector( XMLoadFloat3( &m_position ) );
		mtxRotate = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &m_rotation ) );
		mtxTrans = XMMatrixTranslationFromVector( XMLoadFloat3( &m_translation ) );
		mtxAngle = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &m_angle ) );
		mtxScale = XMMatrixScalingFromVector( XMLoadFloat3( &m_scale ) );

		// 行列計算
		mtxPos = XMMatrixMultiply( mtxRotate, mtxPos );
		mtxPos = XMMatrixMultiply( mtxTrans, mtxPos );
		mtxPos = XMMatrixMultiply( mtxAngle, mtxPos );
		mtxPos = XMMatrixMultiply( mtxScale, mtxPos );

		// ワールド行列の更新
		XMStoreFloat4x4( &m_localWorld, mtxPos );

		// 現在座標を計算後の座標に更新
		m_position.x = m_localWorld._41;
		m_position.y = m_localWorld._42;
		m_position.z = m_localWorld._43;

		// 移動量の初期化
		m_translation = Vector3( 0, 0, 0 );
}