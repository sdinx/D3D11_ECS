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
		m_euler( 0, 0, 0 ),
		m_localEuler( 0, 0, 0 ),
		m_translation( 0, 0, 0 ),
		m_pParent( nullptr )
{
		for ( int i = 0; i < MSG_UPDATE_ALL; i++ )
				m_isMessages[i] = true;
		HandleMessage( GameUtility::Message( MSG_UPDATE_LOCAL ) );
}


Transform::Transform( Transform*  parent ) :
		m_localScale( 1, 1, 1 ),
		m_scale( 1, 1, 1 ),
		m_position( 0, 0, 0 ),
		m_localPosition( 0, 0, 0 ),
		m_euler( 0, 0, 0 ),
		m_localEuler( 0, 0, 0 ),
		m_translation( 0, 0, 0 ),
		m_pParent( parent )
{
		for ( int i = 0; i < MSG_UPDATE_ALL; i++ )
				m_isMessages[i] = true;
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
		
		// ���ݍ��W���v�Z��̍��W�ɍX�V
		m_position.m_floats[0] = m_world._41;
		m_position.m_floats[1] = m_world._42;
		m_position.m_floats[2] = m_world._43;

		// ���[�J����Ԃ��v�Z
		UpdateLocalMatrix();
		// ��Έʒu�����߂�
		mtxWorld = DirectX::XMMatrixMultiply( DirectX::XMLoadFloat4x4( &m_localWorld ), mtxWorld );

		// ��Έʒu��ݒ�
		DirectX::XMStoreFloat4x4( &m_multiplyWorld, mtxWorld );
}


void  Transform::UpdateLocalMatrix()
{
		if ( m_isMessages[MSG_UPDATE_LOCAL] == false )
				return;

		XMMATRIX  localWorld;
		Vector3  r( ToRadian( m_localEuler.m_floats[0] ), ToRadian( m_localEuler.m_floats[1] ), ToRadian( m_localEuler.m_floats[2] ) );

		localWorld = XMMatrixMultiply( XMMatrixRotationRollPitchYawFromVector( r.get128() ), XMMatrixTranslationFromVector( m_localPosition.get128() ) );
		localWorld = XMMatrixMultiply( XMMatrixScalingFromVector( m_localScale.get128() ), localWorld );
		XMStoreFloat4x4( &m_localWorld, localWorld );

		m_isMessages[MSG_UPDATE_LOCAL] = false;
}


void  Transform::UpdateMatrix()
{
		if ( m_isMessages[MSG_UPDATE_MATRIX] == false )
				return;

		XMMATRIX  mtxPos, mtxTrans, mtxRotate, mtxScale;
		Vector3  r( ToRadian( m_euler.m_floats[0] ), ToRadian( m_euler.m_floats[1] ), ToRadian( m_euler.m_floats[2] ) );

		// �s��ϊ�
		mtxPos = XMMatrixTranslationFromVector( m_position.get128() );
		mtxRotate = XMMatrixRotationRollPitchYawFromVector( r.get128() );
		mtxTrans = XMMatrixTranslationFromVector( m_translation.get128() );
		mtxScale = XMMatrixScalingFromVector( m_scale.get128() );

		// �s��v�Z
		mtxPos = XMMatrixMultiply( mtxRotate, mtxPos );
		mtxPos = XMMatrixMultiply( mtxTrans, mtxPos );
		mtxPos = XMMatrixMultiply( mtxScale, mtxPos );

		// ���[���h�s��̍X�V
		XMStoreFloat4x4( &m_world, mtxPos );

		// ���ݍ��W���v�Z��̍��W�ɍX�V
		m_position.m_floats[0] = m_world._41;
		m_position.m_floats[1] = m_world._42;
		m_position.m_floats[2] = m_world._43;

		// �ړ��ʂ̏�����
		m_translation = Vector3( 0, 0, 0 );

		m_isMessages[MSG_UPDATE_LOCAL] = true;
		m_isMessages[MSG_UPDATE_PARENT] = true;
		m_isMessages[MSG_UPDATE_MATRIX] = false;
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

		parent->m_isMessages[MSG_UPDATE_PARENT] = false;

		// �q��Ԃ֍s��v�Z���Ă���
		return  DirectX::XMMatrixMultiply( mtxParent, mtxWorld );
}