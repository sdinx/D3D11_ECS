//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <D3D11Utility\Camera.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;

//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
std::unique_ptr<CONSTANTBUFFER>  Camera::s_pCBuffer = nullptr;

//----------------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------------
struct  ConstantBufferForPerFrame
{
		Matrix4x4  view;
		Matrix4x4  projection;
};


Camera::Camera()
{
		m_eyePosition = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		m_focusTarget = DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
		m_upDirection = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

		UpdateView();
		UpdateProjection( DirectX::XM_PIDIV2, GetAspectRatio(), 0.01f, 1000.0f );
		UpdateConstantBuffer();
}


Camera::Camera( Vector3  eyePosition, Vector3  focusPosition, Vector3  upDirection, FLOAT FovAngleY, FLOAT AspectHByW, FLOAT NearZ, FLOAT FarZ )
{
		m_eyePosition = DirectX::XMLoadFloat3( &eyePosition );
		m_focusTarget = DirectX::XMLoadFloat3( &focusPosition );
		m_upDirection = DirectX::XMLoadFloat3( &upDirection );

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
						UpdateView();
				}
				break;
		case  MSG_UPDATE_CBUFFER:
				{
						UpdateConstantBuffer();
				}
				break;
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
		if ( s_pCBuffer == nullptr )
		{
				s_pCBuffer = std::unique_ptr<CONSTANTBUFFER>( new  CONSTANTBUFFER );
				CreateConstantBuffer( &s_pCBuffer->pCB, s_pCBuffer->nCBSlot, sizeof( ConstantBufferForPerFrame ) );
		}
}


void  Camera::SetPosition( Vector3  eyePosition )
{
		m_eyePosition = DirectX::XMLoadFloat3( &eyePosition );
}


void  Camera::SetTarget( Vector3  focusPosition )
{
		m_focusTarget = DirectX::XMLoadFloat3( &focusPosition );
}


void  Camera::SetUp( Vector3  upDirection )
{
		m_upDirection = DirectX::XMLoadFloat3( &upDirection );
}


void  Camera::Update()
{
		/* NOTHING */
}


void  Camera::UpdateView()
{
		DirectX::XMStoreFloat4x4( &m_view, DirectX::XMMatrixLookAtLH( m_eyePosition, m_focusTarget, m_upDirection ) );
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
		pd3dDeviceContext->UpdateSubresource( s_pCBuffer->pCB, 0, nullptr, &cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( 0, 1, &s_pCBuffer->pCB );
		pd3dDeviceContext->GSSetConstantBuffers( 0, 1, &s_pCBuffer->pCB );
}


void  Camera::Release()
{
		m_eyePosition = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		m_focusTarget = DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
		m_upDirection = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

		UpdateView();
		UpdateProjection( DirectX::XM_PIDIV2, GetAspectRatio(), 0.01f, 1000.0f );
}