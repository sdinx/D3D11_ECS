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


void  PointLight::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( CBufferPointLight ) );
		}
}