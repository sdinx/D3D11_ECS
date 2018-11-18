//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility\components/DirectionLight.h>
#include  <d3d11utility\Systems\ComponentManager.h>

//----------------------------------------------------------------------------------
// using  namespace
//----------------------------------------------------------------------------------
using  namespace  D3D11Utility;
using  namespace  DirectX;

//----------------------------------------------------------------------------------
// static  variables
//----------------------------------------------------------------------------------
ComponentId  DirectionLight::STATIC_COMPONENT_ID = STATIC_ID_INVALID;
const  UINT  DirectionLight::s_nConstantBufferSlot;
ID3D11Buffer*  DirectionLight::s_pConstantBuffer = nullptr;


DirectionLight::DirectionLight( Vector3  direction, Vector3  ambient, Vector3  diffuse, Vector4  specular )
{
		m_cbuffer.direction = direction;
		m_cbuffer.ambient = ambient;
		m_cbuffer.diffuse = diffuse;
		m_cbuffer.specular = specular;
}


DirectionLight::~DirectionLight()
{

}


void  DirectionLight::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( CBufferDirectionLight ) );
		}
}


void  DirectionLight::Update()
{
		UpdateConstantBuffer();
}


void  DirectionLight::UpdateConstantBuffer()
{
		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &m_cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->PSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
}