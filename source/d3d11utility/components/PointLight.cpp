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
const  uint  PointLight::s_nConstantBufferSlot;
const  uint  PointLight::s_nLightCounts;
Camera*  PointLight::s_camera;
PointLight::CBufferPointLight  PointLight::s_instanceLights[NUM_POINT_LIGHT_COUNTS];
Graphics::StructuredBuffer<PointLight::CBufferPointLight>*  PointLight::s_pStructureBuffer;
static  uint  s_lightCounts = 0;


PointLight::PointLight( Vector3  position, float  distance, Graphics::Material  material ) :
		m_nInstanceId( s_lightCounts )
{
		s_lightCounts++;
		m_pRenderer = _Singleton<Systems::IDirect3DRenderer>::GetInstance();

		//	Transform Ç™Ç»Ç¢èÍçáÇÕí«â¡.

		s_instanceLights[m_nInstanceId].position.x = position.m_floats[0];
		s_instanceLights[m_nInstanceId].position.y = position.m_floats[1];
		s_instanceLights[m_nInstanceId].position.z = position.m_floats[2];

		s_instanceLights[m_nInstanceId].distance = distance;
		m_materialId = material.GetMaterialId();
}


PointLight::PointLight( Vector3  position, float  distance, Graphics::MaterialId  id ) :
		m_nInstanceId( s_lightCounts )
{
		s_lightCounts++;
		m_pRenderer = _Singleton<Systems::IDirect3DRenderer>::GetInstance();

		//	Transform Ç™Ç»Ç¢èÍçáÇÕí«â¡.

		s_instanceLights[m_nInstanceId].position.x = position.m_floats[0];
		s_instanceLights[m_nInstanceId].position.y = position.m_floats[1];
		s_instanceLights[m_nInstanceId].position.z = position.m_floats[2];

		s_instanceLights[m_nInstanceId].distance = distance;
		m_materialId = id;
}


PointLight::~PointLight()
{

}


void  PointLight::SetConstantBuffer()
{
		s_pStructureBuffer = new  Graphics::StructuredBuffer<CBufferPointLight>( s_nLightCounts, D3D11_BIND_SHADER_RESOURCE, true );
}


void  PointLight::Update()
{
		Vector3  position = GetComponent<Transform>()->GetPosition();

		s_instanceLights[m_nInstanceId].position.x = position.m_floats[0];
		s_instanceLights[m_nInstanceId].position.y = position.m_floats[1];
		s_instanceLights[m_nInstanceId].position.z = position.m_floats[2];
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