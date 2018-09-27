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
		m_scale( 1, 1, 1 ),
		m_position( 0, 0, 0 ),
		m_localPosition( 0, 0, 0 ),
		m_rotation( 0, 0, 0 ),
		m_localRotation( 0, 0, 0 ),
		m_pParent( nullptr )
{
		HandleMessage( GameUtility::Message( MSG_UPDATE_LOCAL ) );
}


Transform::Transform( Transform*  parent ) :
		m_localScale( 1, 1, 1 ),
		m_scale( 1, 1, 1 ),
		m_position( 0, 0, 0 ),
		m_localPosition( 0, 0, 0 ),
		m_rotation( 0, 0, 0 ),
		m_localRotation( 0, 0, 0 ),
		m_pParent( parent )
{
		HandleMessage( GameUtility::Message( MSG_UPDATE_LOCAL ) );
}


void  Transform::HandleMessage( const  GameUtility::Message&  msg )
{
}


void  Transform::Update()
{
		// ルート内の行列を計算
		DirectX::XMMATRIX  mtxWorld = MultiplyRootTransform( this );
		DirectX::XMStoreFloat4x4( &m_world, mtxWorld );

		// ローカル空間を計算
		UpdateLocalMatrix();
		// 絶対位置を求める
		mtxWorld = DirectX::XMMatrixMultiply( DirectX::XMLoadFloat4x4( &m_localWorld ), mtxWorld );

		XMStoreFloat4x4( &m_world, mtxWorld );
}


void  Transform::UpdateLocalMatrix()
{
		XMMATRIX  localWorld;

		localWorld = XMMatrixMultiply( XMMatrixRotationQuaternion( m_localRotation.get128() ), XMMatrixTranslationFromVector( m_localPosition.get128() ) );
		localWorld = XMMatrixMultiply( XMMatrixScalingFromVector( m_localScale.get128() ), localWorld );
		XMStoreFloat4x4( &m_localWorld, localWorld );
}


void  Transform::UpdateMatrix()
{
		XMMATRIX  mtxPos, mtxTrans, mtxRotate, mtxScale;

		// 行列変換
		mtxPos = XMMatrixTranslationFromVector( m_position.get128() );
		mtxRotate = XMMatrixRotationRollPitchYawFromVector( m_rotation.get128() );
		mtxScale = XMMatrixScalingFromVector( m_scale.get128() );

		// 行列計算
		mtxPos = XMMatrixMultiply( mtxRotate, mtxPos );
		mtxPos = XMMatrixMultiply( mtxTrans, mtxPos );
		mtxPos = XMMatrixMultiply( mtxScale, mtxPos );

		// ワールド行列の更新
		XMStoreFloat4x4( &m_world, mtxPos );
}


DirectX::XMMATRIX  Transform::MultiplyRootTransform( Transform*  parent )
{
		// 親のワールド行列を計算
		parent->UpdateMatrix();

		// 親のワールド行列を取得
		DirectX::XMMATRIX  mtxWorld = DirectX::XMLoadFloat4x4( &parent->GetWorld() );
		DirectX::XMMATRIX  mtxParent;

		// 更に親がある場合
		if ( parent->m_pParent != nullptr )
				mtxParent = MultiplyRootTransform( parent->m_pParent );
		else
				return  mtxWorld;

		// 子空間へ行列計算していく
		return  DirectX::XMMatrixMultiply( mtxParent, mtxWorld );
}