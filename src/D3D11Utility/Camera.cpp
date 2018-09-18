//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Camera.h>
#include  <D3D11Utility\Transform.h>
#include  <D3D11Utility\Systems\ComponentManager.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  DirectX;

//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
ComponentId  Camera::STATIC_COMPONENT_ID = STATIC_ID_INVALID;
const  UINT  Camera::s_nConstantBufferSlot;
ID3D11Buffer*  Camera::s_pConstantBuffer = nullptr;

//----------------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------------
struct  ConstantBufferForPerFrame
{
		Matrix4x4  view;
		Matrix4x4  projection;
};


Camera::Camera( Transform*  transform, Transform*  targetTransform )
{
		assert( transform != nullptr );

		m_targetTransform = targetTransform;

		m_eyePosition = &transform->GetPosition();
		m_translation = &transform->GetTranslation();

		m_focusTarget = Vector3( 0.0f, 0.0f, 1.0f );
		m_upDirection = Vector3( 0.0f, 1.0f, 0.0f );
		*m_eyePosition = Vector3( 0.0f, 1.0f, 0.0f );
		*m_translation = Vector3( 0.0f, 0.0f, 0.0f );
		m_lookRotation = Vector3( 0.0f, 0.0f, 0.0f );

		UpdateView();
		UpdateProjection( DirectX::XM_PIDIV2, GetAspectRatio(), 0.01f, 1000.0f );
		UpdateConstantBuffer();
}


Camera::Camera( Transform*  transform, Vector3  eyePosition, Vector3  focusPosition, Vector3  upDirection, FLOAT FovAngleY, FLOAT AspectHByW, FLOAT NearZ, FLOAT FarZ )
{
		assert( transform != nullptr );

		m_eyePosition = &transform->GetPosition();
		//m_translation = &transform->GetTranslation();

		*m_eyePosition = eyePosition;
		m_focusTarget = focusPosition;
		m_upDirection = upDirection;

		UpdateView();
		UpdateProjection( FovAngleY, AspectHByW, NearZ, FarZ );
}


Camera::~Camera()
{
		Release();
}


void  Camera::HandleMessage( const  GameUtility::Message&  msg )
{
		switch ( msg.messageType )
		{
		case  MSG_UPDATE_VIEW:
				{
						Transform*  trans = m_pComponentManager->GetComponent<Transform>( m_parentsEntityId );
						if ( trans != nullptr )
						{
								trans->Update();
								XMMATRIX  localWorld = XMLoadFloat4x4( &trans->GetWorldMatrix() );

								XMStoreFloat4x4( &m_view, XMMatrixMultiply( XMLoadFloat4x4( &m_view ), localWorld ) );
						}// end if
						UpdateView();
				}
				break;
		case  MSG_UPDATE_CBUFFER:
				{
						UpdateConstantBuffer();
				}
				break;
		case  MSG_UPDATE_ALL:
				{
						GetComponent<Transform>()->SetChange( true, Transform::MSG_UPDATE_MATRIX );
						UpdateView();
						UpdateTargetTransform();
						UpdateConstantBuffer();
				}
		default:
				{
						/* NOTHING */
				}
				return;

		}// end switch

}// end HandleMessage(const Message&)


void  Camera::HandleMessage( const  GameUtility::Message&  msg, Value  var )
{
		/* NOTHING */
}// end HandleMessage(const Message&, void*)


void  Camera::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( ConstantBufferForPerFrame ) );
		}
}


void  Camera::SetPosition( Vector3  eyePosition )
{
		*m_eyePosition = eyePosition;
}


void  Camera::SetTarget( Vector3  focusPosition )
{
		m_focusTarget = focusPosition;
}


void  Camera::SetRotation( float  x, float  y, float  z )
{

}


void  Camera::SetLookRotation( float  x, float  y, float  z )
{
		m_lookRotation.x += x;
		m_lookRotation.y += y;
		m_lookRotation.z += z;

		GetComponent<Transform>()->SetEuler( m_lookRotation );

		if ( m_lookRotation.x > 89.99f || m_lookRotation.x < -90.0f )
				m_lookRotation.x -= x;
}


void  Camera::SetTranslation( Vector3  trans )
{
		*m_translation += trans;
}


void  Camera::SetUp( Vector3  upDirection )
{
		m_upDirection = upDirection;
}


void  Camera::Update()
{
		/* NOTHING */
}


void  Camera::UpdateView()
{
		Matrix4x4  dist;
		Vector3  focusPos( m_focusTarget );
		Vector3  r( ToRadian( m_lookRotation.x ), ToRadian( m_lookRotation.y ), ToRadian( 0 ) );
		Vector3  r2( ToRadian( 0 ), ToRadian( m_lookRotation.y ), ToRadian( 0 ) );

		auto  mtxPos = DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( m_eyePosition ) );
		auto  mtxTrans = DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( m_translation ) );
		auto  mtxRotate = DirectX::XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &r2 ) );
		auto  mtxAngle = DirectX::XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( &r ) );

		// 行列計算
		mtxPos = XMMatrixMultiply( mtxAngle, mtxPos );
		// ワールド行列の更新
		XMStoreFloat4x4( &m_localWorld, XMMatrixMultiply( mtxTrans, mtxPos ) );

		// カメラ位置を更新
		m_eyePosition->x = m_localWorld._41;
		m_eyePosition->y = m_localWorld._42;
		m_eyePosition->z = m_localWorld._43;

		auto  mtxDist = DirectX::XMMatrixTranslationFromVector( XMLoadFloat3( &focusPos ) );
		mtxDist = XMMatrixMultiply( mtxDist, mtxPos );
		mtxDist = XMMatrixMultiply( mtxTrans, mtxDist );

		// ワールド行列の更新
		XMStoreFloat4x4( &dist, mtxDist );

		focusPos.x = dist._41;
		focusPos.y = dist._42;
		focusPos.z = dist._43;

		// ビュー行列変換
		DirectX::XMStoreFloat4x4(
				&m_view,
				DirectX::XMMatrixLookAtLH( 
						DirectX::XMLoadFloat3( m_eyePosition ),
						DirectX::XMLoadFloat3( &focusPos ),
						DirectX::XMLoadFloat3( &m_upDirection ) ) );

}


void  Camera::UpdateProjection( FLOAT fovAngleY, FLOAT aspectHByW, FLOAT nearZ, FLOAT farZ )
{
		DirectX::XMStoreFloat4x4( &m_projection, DirectX::XMMatrixPerspectiveFovLH( fovAngleY, aspectHByW, nearZ, farZ ) );
}


void  Camera::UpdateConstantBuffer()
{
		ConstantBufferForPerFrame  cbuffer;
		cbuffer.view = GetMatrix4x4View();
		cbuffer.projection = GetMatrix4x4Projection();
		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );

}


//----------------------------------------------------------------------------------
//! @func:     UpdateTargetTransform( void ) : void
//! @brief:    FPSカメラ用の注視点更新関数.
//----------------------------------------------------------------------------------
void  Camera::UpdateTargetTransform()
{
		if ( m_targetTransform == nullptr )
				return;

		m_targetTransform->Update();
		Matrix4x4  world = m_targetTransform->GetWorld();

		Vector3  pos = m_targetTransform->GetLocalPosition();

		auto  mtxAngle = DirectX::XMMatrixRotationRollPitchYawFromVector( DirectX::XMLoadFloat3( &m_lookRotation ) );
		auto  mtxPos = DirectX::XMMatrixTranslationFromVector( DirectX::XMLoadFloat3( &pos) );

		mtxPos = DirectX::XMMatrixMultiply( mtxAngle, mtxPos );
		//mtxPos = DirectX::XMMatrixMultiply( mtxPos, DirectX::XMLoadFloat4x4( &world ) );
		DirectX::XMStoreFloat4x4( &world, mtxPos );

		// TODO: ワールド行列が一時的なものである為更新しても Transform::Update() で更新される.
		m_targetTransform->SetEuler( m_lookRotation );
		//m_targetTransform->SetPosition( world._41, world._42, world._43 );
		m_targetTransform->SetPosition( pos + *m_eyePosition );
}


void  Camera::Release()
{
		*m_eyePosition = Vector3( 0.0f, 1.0f, 0.0f );
		m_focusTarget = Vector3( 0.0f, 0.0f, 1.0f );
		m_upDirection = Vector3( 0.0f, 1.0f, 0.0f );

		UpdateView();
		UpdateProjection( DirectX::XM_PIDIV2, GetAspectRatio(), 0.01f, 1000.0f );
}