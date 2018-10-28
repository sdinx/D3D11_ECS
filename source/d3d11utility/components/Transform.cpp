//----------------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------------
#include  <d3d11utility/components/Transform.h>


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
		m_up( 0, 1, 0 ),
		m_forward( 0, 0, 1 ),
		m_right( 1, 0, 0 ),
		m_pParent( nullptr )
{
		m_up = m_up.normalized();
		m_forward = m_up.normalized();
		m_right = m_up.normalized();

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
		m_up = m_up.normalized();
		m_forward = m_up.normalized();
		m_right = m_up.normalized();

}


void  Transform::HandleMessage( const  Message&  msg )
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
		mtxWorld = DirectX::XMMatrixMultiply( mtxWorld , DirectX::XMLoadFloat4x4( &m_localWorld ) );

		XMStoreFloat4x4( &m_world, mtxWorld );
}


void  Transform::UpdateLocalMatrix()
{
		XMMATRIX  mtxWorld;

		Matrix3x3  matRotate;
		matRotate.setRotation( m_localRotation );

		mtxWorld.r[0].m128_f32[0] = m_localScale.m_floats[0] * matRotate[0].m_floats[0];
		mtxWorld.r[0].m128_f32[1] = m_localScale.m_floats[0] * matRotate[0].m_floats[1];
		mtxWorld.r[0].m128_f32[2] = m_localScale.m_floats[0] * matRotate[0].m_floats[2];

		mtxWorld.r[1].m128_f32[0] = m_localScale.m_floats[1] * matRotate[1].m_floats[0];
		mtxWorld.r[1].m128_f32[1] = m_localScale.m_floats[1] * matRotate[1].m_floats[1];
		mtxWorld.r[1].m128_f32[2] = m_localScale.m_floats[1] * matRotate[1].m_floats[2];

		mtxWorld.r[2].m128_f32[0] = m_localScale.m_floats[2] * matRotate[2].m_floats[0];
		mtxWorld.r[2].m128_f32[1] = m_localScale.m_floats[2] * matRotate[2].m_floats[1];
		mtxWorld.r[2].m128_f32[2] = m_localScale.m_floats[2] * matRotate[2].m_floats[2];

		mtxWorld.r[3].m128_f32[0] = m_localPosition.m_floats[0];
		mtxWorld.r[3].m128_f32[1] = m_localPosition.m_floats[1];
		mtxWorld.r[3].m128_f32[2] = m_localPosition.m_floats[2];

		mtxWorld.r[0].m128_f32[3] = 0.0f;
		mtxWorld.r[1].m128_f32[3] = 0.0f;
		mtxWorld.r[2].m128_f32[3] = 0.0f;
		mtxWorld.r[3].m128_f32[3] = 1.0f;

		XMStoreFloat4x4( &m_localWorld, mtxWorld );
}


void  Transform::UpdateMatrix()
{
		XMMATRIX  mtxWorld;

		Matrix3x3  matRotate;
		matRotate.setRotation( m_rotation );

		mtxWorld.r[0].m128_f32[0] = m_scale.m_floats[0] * matRotate[0].m_floats[0];
		mtxWorld.r[0].m128_f32[1] = m_scale.m_floats[0] * matRotate[0].m_floats[1];
		mtxWorld.r[0].m128_f32[2] = m_scale.m_floats[0] * matRotate[0].m_floats[2];

		mtxWorld.r[1].m128_f32[0] = m_scale.m_floats[1] * matRotate[1].m_floats[0];
		mtxWorld.r[1].m128_f32[1] = m_scale.m_floats[1] * matRotate[1].m_floats[1];
		mtxWorld.r[1].m128_f32[2] = m_scale.m_floats[1] * matRotate[1].m_floats[2];

		mtxWorld.r[2].m128_f32[0] = m_scale.m_floats[2] * matRotate[2].m_floats[0];
		mtxWorld.r[2].m128_f32[1] = m_scale.m_floats[2] * matRotate[2].m_floats[1];
		mtxWorld.r[2].m128_f32[2] = m_scale.m_floats[2] * matRotate[2].m_floats[2];

		mtxWorld.r[3].m128_f32[0] = m_position.m_floats[0];
		mtxWorld.r[3].m128_f32[1] = m_position.m_floats[1];
		mtxWorld.r[3].m128_f32[2] = m_position.m_floats[2];

		mtxWorld.r[0].m128_f32[3] = 0.0f;
		mtxWorld.r[1].m128_f32[3] = 0.0f;
		mtxWorld.r[2].m128_f32[3] = 0.0f;
		mtxWorld.r[3].m128_f32[3] = 1.0f;

		// ワールド行列の更新
		XMStoreFloat4x4( &m_world, mtxWorld );
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