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


void  SpotLight::SetConstantBuffer()
{
		if ( s_pConstantBuffer == nullptr )
		{
				CreateConstantBuffer( s_pConstantBuffer, sizeof( CBufferSpotLight ) );
		}
}