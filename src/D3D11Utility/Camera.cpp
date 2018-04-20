//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <Camera.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;

//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
CONSTANTBUFFER*  Camera::s_pCBuffer = nullptr;

//----------------------------------------------------------------------------------
// struct
//----------------------------------------------------------------------------------
struct  ConstantBufferForPerFrame
{
		DirectX::XMMATRIX  view;
		DirectX::XMMATRIX  projection;
};


Camera::Camera()
{
		m_eyePosition = DirectX::XMVectorZero(); 
		m_focusTarget = DirectX::XMVectorZero();
		m_upDirection = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

		UpdateView();
		UpdateProjection( DirectX::XM_PIDIV2, GetAspectRatio(), 0.01f, 1000.0f );
}


Camera::Camera( Vector3  eyePosition, Vector3  focusPosition, Vector3  upDirection, float FovAngleY, float AspectHByW, float NearZ, float FarZ )
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


VOID  Camera::SetConstantBuffer()
{
		if ( s_pCBuffer == nullptr )
		{
				s_pCBuffer = new  CONSTANTBUFFER;
				CreateConstantBuffer( s_pCBuffer->pCB, s_pCBuffer->nCBSlot, sizeof( ConstantBufferForPerFrame ) );
		}
}


VOID  Camera::UpdateView()
{
		DirectX::XMVECTOR  eye = DirectX::XMVectorSet( 0, 1, 0, 0 );
		DirectX::XMVECTOR  focus = DirectX::XMVectorSet( 0, 0, 100, 0 );
		DirectX::XMVECTOR  up = DirectX::XMVectorSet( 0, 1, 0, 0 );

		m_view = DirectX::XMMatrixLookAtLH( eye, focus, up );
}


VOID  Camera::UpdateProjection( float fovAngleY, float aspectHByW, float nearZ, float farZ )
{
		m_projection = DirectX::XMMatrixPerspectiveFovLH( fovAngleY, aspectHByW, nearZ, farZ );
}


VOID  Camera::UpdateConstantBuffer()
{
		ConstantBufferForPerFrame  cbuffer;
		cbuffer.view = GetMatrixView();
		cbuffer.projection = GetMatrixProjection();
		pd3dDeviceContext->UpdateSubresource( s_pCBuffer->pCB, 0, nullptr, &cbuffer, 0, 0 );


		pd3dDeviceContext->VSSetConstantBuffers( s_pCBuffer->nCBSlot, 1, &s_pCBuffer->pCB );
}


VOID  Camera::SetPosition( Vector3  eyePosition )
{
		m_eyePosition = DirectX::XMLoadFloat3( &eyePosition );
}


VOID  Camera::SetTarget( Vector3  focusPosition )
{
		m_focusTarget = DirectX::XMLoadFloat3( &focusPosition );
}


VOID  Camera::SetUp( Vector3  upDirection )
{
		m_upDirection = DirectX::XMLoadFloat3( &upDirection );
}


Matrix4x4  Camera::GetMatrix4x4Projection() 
{ 
		Matrix4x4  retProjection;
		DirectX::XMStoreFloat4x4( &retProjection, m_projection );
		return  retProjection;
}


Matrix4x4  Camera::GetMatrix4x4View() 
{
		Matrix4x4  retView;
		DirectX::XMStoreFloat4x4( &retView, m_view );
		return  retView;
}


VOID  Camera::Release()
{
		m_eyePosition = DirectX::XMVectorZero(); 
		m_focusTarget = DirectX::XMVectorZero();
		m_upDirection = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

		UpdateView();
		UpdateProjection( DirectX::XM_PIDIV2, GetAspectRatio(), 0.01f, 1000.0f );
}