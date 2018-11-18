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


SpotLight::SpotLight( Vector3  position, Vector3  ambient, Vector3  diffuse, Vector4  specular, Vector3  attenuate )
{
		m_cbuffer.position = position;
		m_cbuffer.ambient = ambient;
		m_cbuffer.diffuse = diffuse;
		m_cbuffer.specular = specular;
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

}