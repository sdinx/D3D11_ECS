//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/PointLight.h>
#include  <d3d11utility\Systems\ComponentManager.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  DirectX;

//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
ComponentId  PointLight::STATIC_COMPONENT_ID = STATIC_ID_INVALID;
const  UINT  PointLight::s_nConstantBufferSlot;
ID3D11Buffer*  PointLight::s_pConstantBuffer = nullptr;


PointLight::PointLight( Vector3  position, Vector3  ambient, Vector3  diffuse, Vector4  specular, Vector3  attenuate )
{
		m_cbuffer.position = position;
		m_cbuffer.ambient = ambient;
		m_cbuffer.diffuse = diffuse;
		m_cbuffer.specular = specular;
		m_cbuffer.attenuate = attenuate;
}


PointLight::~PointLight()
{

}


void  PointLight::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( CBufferPointLight ) );
		}
}


void  PointLight::Update()
{
		UpdateConstantBuffer();
}


void  PointLight::UpdateConstantBuffer()
{
		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &m_cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->PSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
}