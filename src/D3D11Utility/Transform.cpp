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

		// �s��ϊ�
		mtxPos = XMMatrixTranslationFromVector( XMLoadFloat3( &m_position ) );
		mtxRotate = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &m_rotation ) );
		mtxTrans = XMMatrixTranslationFromVector( XMLoadFloat3( &m_translation ) );
		mtxAngle = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &m_angle ) );
		mtxScale = XMMatrixScalingFromVector( XMLoadFloat3( &m_scale ) );

		// �s��v�Z
		mtxPos = XMMatrixMultiply( mtxRotate, mtxPos );
		mtxPos = XMMatrixMultiply( mtxTrans, mtxPos );
		mtxPos = XMMatrixMultiply( mtxAngle, mtxPos );
		mtxPos = XMMatrixMultiply( mtxScale, mtxPos );

		// ���[���h�s��̍X�V
		XMStoreFloat4x4( &m_localWorld, mtxPos );

		// ���ݍ��W���v�Z��̍��W�ɍX�V
		m_position.x = m_localWorld._41;
		m_position.y = m_localWorld._42;
		m_position.z = m_localWorld._43;

		// �ړ��ʂ̏�����
		m_translation = Vector3( 0, 0, 0 );
}