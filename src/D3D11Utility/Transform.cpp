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
		// ���[�g���̍s����v�Z
		DirectX::XMMATRIX  mtxWorld = MultiplyRootTransform( this );
		DirectX::XMStoreFloat4x4( &m_world, mtxWorld );

		// ���[�J����Ԃ��v�Z
		UpdateLocalMatrix();
		// ��Έʒu�����߂�
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

		// �s��ϊ�
		mtxPos = XMMatrixTranslationFromVector( m_position.get128() );
		mtxRotate = XMMatrixRotationRollPitchYawFromVector( m_rotation.get128() );
		mtxScale = XMMatrixScalingFromVector( m_scale.get128() );

		// �s��v�Z
		mtxPos = XMMatrixMultiply( mtxRotate, mtxPos );
		mtxPos = XMMatrixMultiply( mtxTrans, mtxPos );
		mtxPos = XMMatrixMultiply( mtxScale, mtxPos );

		// ���[���h�s��̍X�V
		XMStoreFloat4x4( &m_world, mtxPos );
}


DirectX::XMMATRIX  Transform::MultiplyRootTransform( Transform*  parent )
{
		// �e�̃��[���h�s����v�Z
		parent->UpdateMatrix();

		// �e�̃��[���h�s����擾
		DirectX::XMMATRIX  mtxWorld = DirectX::XMLoadFloat4x4( &parent->GetWorld() );
		DirectX::XMMATRIX  mtxParent;

		// �X�ɐe������ꍇ
		if ( parent->m_pParent != nullptr )
				mtxParent = MultiplyRootTransform( parent->m_pParent );
		else
				return  mtxWorld;

		// �q��Ԃ֍s��v�Z���Ă���
		return  DirectX::XMMatrixMultiply( mtxParent, mtxWorld );
}