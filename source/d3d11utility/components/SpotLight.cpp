//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/SpotLight.h>
#include  <d3d11utility\Systems\ComponentManager.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  DirectX;

//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
ComponentId  SpotLight::STATIC_COMPONENT_ID = STATIC_ID_INVALID;
const  UINT  SpotLight::s_nConstantBufferSlot;
ID3D11Buffer*  SpotLight::s_pConstantBuffer = nullptr;


SpotLight::SpotLight( Vector3  position, Vector3  attenuate )
{
		//	Transform Ç™Ç»Ç¢èÍçáÇÕí«â¡.
		if ( ( m_transform = GetComponent<Transform>() ) == nullptr )
				m_transform = AddComponent<Transform>();

		m_transform->SetPosition( position );
		m_cbuffer.position = position;
		m_cbuffer.attenuate = attenuate;
}


SpotLight::~SpotLight()
{

}


void  SpotLight::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( CBufferSpotLight ) );
		}
}


void  SpotLight::Update()
{

}


void  SpotLight::UpdateConstantBuffer()
{
		m_cbuffer.position = m_transform->GetPosition();

		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &m_cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->PSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
}