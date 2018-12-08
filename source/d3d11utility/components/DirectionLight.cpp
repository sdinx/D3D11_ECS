//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility/components/DirectionLight.h>
#include  <d3d11utility/graphics/Material.h>
#include  <d3d11utility/Systems/ComponentManager.h>
#include  <d3d11utility/Systems/IDirect3DRenderer.h>

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


DirectionLight::DirectionLight( Vector3  direction, Graphics::Material  material )
{
		m_pRenderer = _Singleton<Systems::IDirect3DRenderer>::GetInstance();
		m_cbuffer.direction = direction;
		m_materialId = material.GetMaterialId();
}


DirectionLight::DirectionLight( Vector3  direction, Graphics::MaterialId  id )
{
		m_pRenderer = _Singleton<Systems::IDirect3DRenderer>::GetInstance();
		m_cbuffer.direction = direction;
		m_materialId = id;
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


void  DirectionLight::SetMaterial( Graphics::Material  material )
{
		m_materialId = material.GetMaterialId();
}


void  DirectionLight::SetMaterial( Graphics::MaterialId  id )
{
		m_materialId = id;
}


Graphics::Material*  DirectionLight::GetMaterial()
{
		return  m_pRenderer->GetMaterial( m_materialId );
}