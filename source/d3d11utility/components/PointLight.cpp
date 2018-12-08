//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include  <d3d11utility/components/PointLight.h>
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
ComponentId  PointLight::STATIC_COMPONENT_ID = STATIC_ID_INVALID;
const  UINT  PointLight::s_nConstantBufferSlot;
ID3D11Buffer*  PointLight::s_pConstantBuffer = nullptr;


PointLight::PointLight( Vector3  position, Vector3  attenuate, Graphics::Material  material )
{
		m_pRenderer = _Singleton<Systems::IDirect3DRenderer>::GetInstance();

		//	Transform Ç™Ç»Ç¢èÍçáÇÕí«â¡.

		m_cbuffer.position = position;
		m_cbuffer.attenuate = attenuate;
		m_materialId = material.GetMaterialId();
}


PointLight::PointLight( Vector3  position, Vector3  attenuate, Graphics::MaterialId  id )
{
		m_pRenderer = _Singleton<Systems::IDirect3DRenderer>::GetInstance();

		//	Transform Ç™Ç»Ç¢èÍçáÇÕí«â¡.

		m_cbuffer.position = position;
		m_cbuffer.attenuate = attenuate;
		m_materialId = id;
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
		m_cbuffer.position = GetComponent<Transform>()->GetPosition();

		pd3dDeviceContext->UpdateSubresource( s_pConstantBuffer, 0, nullptr, &m_cbuffer, 0, 0 );
		pd3dDeviceContext->VSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->GSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
		pd3dDeviceContext->PSSetConstantBuffers( s_nConstantBufferSlot, 1, &s_pConstantBuffer );
}


void  PointLight::SetMaterial( Graphics::Material  material )
{
		m_materialId = material.GetMaterialId();
}


void  PointLight::SetMaterial( Graphics::MaterialId  id )
{
		m_materialId = id;
}


Graphics::Material*  PointLight::GetMaterial()
{
		return  m_pRenderer->GetMaterial( m_materialId );
}