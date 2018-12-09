//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/Camera.h>
#include  <d3d11utility\components/Transform.h>
#include  <d3d11utility\Systems\ComponentManager.h>

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
		Matrix4x4  invView;
		Matrix4x4  invProjection;
		DirectX::XMVECTOR  cameraPos;
};


Camera::Camera() :
		m_focusPosition( 0.0f, 0.0f, 1.0f ),
		m_upDirection( 0.0f, 1.0f, 0.0f )
{

}


Camera::Camera( Transform*  transform, Transform*  targetTransform ) :
		m_focusPosition( 0.0f, 0.0f, 1.0f ),
		m_upDirection( 0.0f, 1.0f, 0.0f )
{
		assert( transform != nullptr );

		m_transform = transform;

		UpdateView();
		UpdateProjection( DirectX::XM_PIDIV2, GetAspectRatio(), 0.1f, 1000.0f );
		UpdateConstantBuffer();
}


Camera::Camera( Transform*  transform, Vector3  eyePosition, Vector3  focusPosition, Vector3  upDirection, FLOAT FovAngleY, FLOAT AspectHByW, FLOAT NearZ, FLOAT FarZ ) :
		m_focusPosition( 0.0f, 0.0f, 1.0f ),
		m_upDirection( 0.0f, 1.0f, 0.0f )
{
		assert( transform != nullptr );

		m_transform = transform;

		m_focusPosition = focusPosition;
		m_upDirection = upDirection;

		UpdateView();
		UpdateProjection( FovAngleY, AspectHByW, NearZ, FarZ );
}


Camera::~Camera()
{
		Release();
}


void  Camera::HandleMessage( const  Message&  msg )
{
		switch ( msg.messageType )
		{
		case  MSG_UPDATE_VIEW:
				{
						Transform*  trans = m_pComponentManager->GetComponent<Transform>( m_parentsEntityId );
						if ( trans != nullptr )
						{
								XMMATRIX  localWorld = XMLoadFloat4x4( &trans->GetWorld() );

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
						UpdateView();
						UpdateConstantBuffer();
				}
		default:
				{
						/* NOTHING */
				}
				return;

		}// end switch

}// end HandleMessage(const Message&)


void  Camera::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( ConstantBufferForPerFrame ) );
		}
}


void  Camera::Update()
{
		UpdateView();
		UpdateConstantBuffer();
}


void  Camera::UpdateView()
{
		Matrix4x4  mtxWorld = m_transform->GetWorld();

		auto  mtxFocus = DirectX::XMMatrixTranslationFromVector( m_focusPosition.get128() );
		mtxFocus = XMMatrixMultiply( mtxFocus, XMLoadFloat4x4( &mtxWorld ) );
		
		// ƒrƒ…[s—ñ•ÏŠ·
		DirectX::XMStoreFloat4x4(
				&m_view,
				DirectX::XMMatrixLookAtLH(
						m_transform->GetPosition().get128(),
						mtxFocus.r[3],
						m_upDirection.get128() ) );

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
		cbuffer.cameraPos = m_transform->GetPosition().get128();
		XMVECTOR  vec;
		XMStoreFloat4x4( &cbuffer.invView, XMMatrixInverse( &vec, XMLoadFloat4x4( &cbuffer.view ) ) );
		XMStoreFloat4x4( &cbuffer.invProjection, XMMatrixInverse( &vec, XMLoadFloat4x4( &cbuffer.projection ) ) );

		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->PSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );

}


void  Camera::Release()
{
		m_focusPosition = Vector3( 0.0f, 0.0f, 1.0f );
		m_upDirection = Vector3( 0.0f, 1.0f, 0.0f );

		//UpdateView();
		//UpdateProjection( DirectX::XM_PIDIV2, GetAspectRatio(), 0.01f, 1000.0f );
}