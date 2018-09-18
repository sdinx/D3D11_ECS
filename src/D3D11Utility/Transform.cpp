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
		for ( int i = 0; i < MSG_UPDATE_ALL; i++ )
				m_isMessages[i] = true;
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
		m_position.x = m_world._41;
		m_position.y = m_world._42;
		m_position.z = m_world._43;

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
		Vector3  r( ToRadian( m_localEuler.x ), ToRadian( m_localEuler.y ), ToRadian( m_localEuler.z ) );

		localWorld = XMMatrixMultiply( XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &r ) ), XMMatrixTranslationFromVector( XMLoadFloat3( &m_localPosition ) ) );
		localWorld = XMMatrixMultiply( XMMatrixScalingFromVector( XMLoadFloat3( &m_localScale ) ), localWorld );
		XMStoreFloat4x4( &m_localWorld, localWorld );

		m_isMessages[MSG_UPDATE_LOCAL] = false;
}


void  Transform::UpdateMatrix()
{
		if ( m_isMessages[MSG_UPDATE_MATRIX] == false )
				return;

		XMMATRIX  mtxPos, mtxTrans, mtxRotate, mtxScale;
		Vector3  r( ToRadian( m_euler.x ), ToRadian( m_euler.y ), ToRadian( m_euler.z ) );

		// �s��ϊ�
		mtxPos = XMMatrixTranslationFromVector( XMLoadFloat3( &m_position ) );
		mtxRotate = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &r ) );
		mtxTrans = XMMatrixTranslationFromVector( XMLoadFloat3( &m_translation ) );
		mtxScale = XMMatrixScalingFromVector( XMLoadFloat3( &m_scale ) );

		// �s��v�Z
		mtxPos = XMMatrixMultiply( mtxRotate, mtxPos );
		mtxPos = XMMatrixMultiply( mtxTrans, mtxPos );
		mtxPos = XMMatrixMultiply( mtxScale, mtxPos );

		// ���[���h�s��̍X�V
		XMStoreFloat4x4( &m_world, mtxPos );

		// ���ݍ��W���v�Z��̍��W�ɍX�V
		m_position.x = m_world._41;
		m_position.y = m_world._42;
		m_position.z = m_world._43;

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